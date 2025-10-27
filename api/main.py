import time
import redis
from enum import Enum
from typing import Optional
from fastapi import FastAPI, HTTPException, status
from pydantic import BaseModel, model_validator, Field

class Symbol(str, Enum):
    BTC_USDT = "btc-usdt"
    ETH_USDT = "eth-usdt"

class OrderType(str, Enum):
    MARKET = "MARKET"
    LIMIT = "LIMIT"

class TimeInForce(str, Enum):
    GTC = "GTC"  # Good 'Til Canceled
    IOC = "IOC"  # Immediate or Cancel
    FOK = "FOK"  # Fill or Kill

class Side(str, Enum):
    BUY = "BUY"
    SELL = "SELL"

class Order(BaseModel):
    symbol: Symbol
    order_type: OrderType
    time_in_force: TimeInForce
    side: Side
    quantity: float = Field(gt=0, description="Order quantity must be greater than 0")
    price: Optional[float] = Field(None, gt=0, description="Price must be greater than 0 if provided")

    @model_validator(mode='after')
    def validate_order_logic(self) -> 'Order':
        # LIMIT orders must have a price.
        if self.order_type == OrderType.LIMIT and self.price is None:
            raise ValueError("Price must be provided for LIMIT orders.")

        # MARKET orders must not have a price (it will be ignored, so we clear it).
        if self.order_type == OrderType.MARKET and self.price is not None:
            self.price = None

        # MARKET orders cannot be GTC.
        if self.order_type == OrderType.MARKET and self.time_in_force == TimeInForce.GTC:
            raise ValueError("MARKET orders cannot have a TimeInForce of GTC.")

        return self

app = FastAPI(
    title="Order Submission API",
    description="A simple API to submit trading orders to a Redis queue.",
)

try:
    redis_client = redis.Redis(
        host='redis', 
        port=6379, 
        decode_responses=True
    )
    redis_client.ping()
    print("Successfully connected to Redis at redis:6379")
except redis.exceptions.ConnectionError as e:
    print(f"Error connecting to Redis: {e}")
    redis_client = None

@app.get("/", summary="Health Check")
def read_root():
    """
    Root endpoint to check if the API is running.
    """
    return {"status": "API is running"}

@app.post("/submit_order", status_code=status.HTTP_201_CREATED, summary="Submit a New Order")
def submit_order(order: Order):
    """
    Receives an order, validates it, formats it, and LPUSHes it to the
    'incoming_orders' Redis queue.
    """
    if not redis_client:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Redis connection not available.",
        )

    try:
        epoch_us = int(time.time() * 1_000_000)

        price_int_str = str(int(order.price * 100)) if order.order_type == OrderType.LIMIT else "0"
        quantity_int_str = str(int(order.quantity * 100))

        order_string = (
            f"{epoch_us} {epoch_us} {order.symbol.value} {price_int_str} "
            f"{quantity_int_str} {order.order_type.value} {order.side.value} "
            f"{order.time_in_force.value}"
        )
        print("Order String:", order_string)

        redis_client.lpush("incoming_orders", order_string)
        
        return {
            "message": "Order submitted successfully.",
            "order_id": epoch_us,
            "order_string": order_string
        }
        
    except redis.exceptions.ConnectionError as e:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail=f"Failed to push order to Redis: {e}",
        )
    except Exception as e:
        raise HTTPException(
            status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
            detail=f"An unexpected error occurred: {e}",
        )

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
