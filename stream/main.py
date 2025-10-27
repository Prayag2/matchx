import asyncio
import redis.asyncio as redis
from fastapi import FastAPI, WebSocket, WebSocketDisconnect

app = FastAPI(
    title="Real-time Data Stream API",
    description="Streams market and trade data via WebSockets from Redis Pub/Sub.",
)

try:
    redis_client = redis.from_url(
        "redis://redis:6379", 
        decode_responses=True
    )
    print("Async Redis client configured for redis:6379")
except Exception as e:
    print(f"Error configuring async Redis: {e}")
    redis_client = None

async def redis_listener(websocket: WebSocket, channel_name: str):
    """
    Handles subscribing to a Redis channel and pushing messages to a WebSocket.
    """
    if not redis_client:
        await websocket.close(code=1011, reason="Redis connection not available.")
        return

    pubsub = None
    try:
        # Create a new pubsub instance
        pubsub = redis_client.pubsub()
        await pubsub.subscribe(channel_name)
        print(f"Client {websocket.client} subscribed to {channel_name}")

        # Listen for messages and send to client
        while True:
            message = await pubsub.get_message(ignore_subscribe_messages=True, timeout=0)
            
            if message:
                await websocket.send_text(message['data'])

    except WebSocketDisconnect:
        print(f"Client {websocket.client} disconnected from {channel_name}.")
    except Exception as e:
        print(f"An error occurred for client {websocket.client} on {channel_name}: {e}")
        await websocket.close(code=1011, reason=f"An error occurred: {e}")
    finally:
        if pubsub:
            try:
                await pubsub.unsubscribe(channel_name)
                await pubsub.close()
                print(f"Client {websocket.client} unsubscribed from {channel_name}.")
            except Exception as e:
                print(f"Error during Redis pubsub cleanup: {e}")

@app.websocket("/ws/market_data")
async def websocket_market_data(websocket: WebSocket):
    """
    WebSocket endpoint for streaming market data.
    Subscribes to "channel:market".
    """
    await websocket.accept()
    await redis_listener(websocket, "channel:market")

@app.websocket("/ws/trade_execution")
async def websocket_trade_execution(websocket: WebSocket):
    """
    WebSocket endpoint for streaming trade execution data.
    Subscribes to "channel:trade_execution".
    """
    await websocket.accept()
    await redis_listener(websocket, "channel:trade_execution")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8001)
