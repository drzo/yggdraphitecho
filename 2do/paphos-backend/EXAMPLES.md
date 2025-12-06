# API Usage Examples

This document provides practical examples of using the Paphos Backend API.

## Prerequisites

- API server running at `http://localhost:3000`
- `curl` or similar HTTP client
- `jq` for pretty JSON output (optional)

## Quick Start Flow

### 1. Register a New User

```bash
curl -X POST http://localhost:3000/api/v1/users/register \
  -H "Content-Type: application/json" \
  -d '{
    "user": {
      "email": "alice@example.com",
      "password": "SecurePassword123",
      "password_confirmation": "SecurePassword123"
    }
  }'
```

**Response:**
```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
}
```

**Save this token for subsequent requests!**

### 2. Login (Existing User)

```bash
curl -X POST http://localhost:3000/api/v1/users/login \
  -H "Content-Type: application/json" \
  -d '{
    "user": {
      "email": "alice@example.com",
      "password": "SecurePassword123"
    }
  }'
```

### 3. Get Current User Profile

```bash
TOKEN="your_token_here"

curl -X GET http://localhost:3000/api/v1/users/me \
  -H "Authorization: Bearer $TOKEN"
```

## Working with Characters

### Create a Character

```bash
curl -X POST http://localhost:3000/api/v1/characters \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "character": {
      "name": "Sophia",
      "description": "A friendly AI assistant who loves to help",
      "greeting": "Hello! I'\''m Sophia. How can I assist you today?",
      "persona": "I am a helpful, friendly, and knowledgeable AI assistant. I enjoy answering questions and having conversations about various topics.",
      "world_scenario": "A modern virtual assistant in a digital workspace",
      "example_chats": "User: What'\''s the weather?\nSophia: I'\''d be happy to help! However, I don'\''t have real-time weather data.",
      "visibility": "public",
      "is_contentious": false
    }
  }'
```

**Response:**
```json
{
  "character": {
    "slug": "sophia",
    "name": "Sophia",
    "description": "A friendly AI assistant who loves to help",
    ...
  }
}
```

### List All Public Characters

```bash
# First page, 20 items
curl -X GET "http://localhost:3000/api/v1/characters?page=1&per_page=20" \
  -H "Authorization: Bearer $TOKEN"

# Filter non-contentious characters only
curl -X GET "http://localhost:3000/api/v1/characters?contentious=false" \
  -H "Authorization: Bearer $TOKEN"
```

### Get a Specific Character

```bash
curl -X GET http://localhost:3000/api/v1/characters/sophia \
  -H "Authorization: Bearer $TOKEN"
```

### Update a Character

```bash
curl -X PATCH http://localhost:3000/api/v1/characters/sophia \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "character": {
      "description": "An even more friendly AI assistant!",
      "visibility": "unlisted"
    }
  }'
```

### Delete a Character

```bash
# Note: Will fail if character is used in any chats
curl -X DELETE http://localhost:3000/api/v1/characters/sophia \
  -H "Authorization: Bearer $TOKEN"
```

## Working with Chats

### Create a Chat with Characters

```bash
# First, create another character
curl -X POST http://localhost:3000/api/v1/characters \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "character": {
      "name": "Marcus",
      "description": "A wise philosopher who loves deep conversations",
      "greeting": "Greetings, friend. What shall we ponder today?",
      "persona": "I am a thoughtful philosopher who enjoys exploring ideas and engaging in meaningful dialogue.",
      "visibility": "public",
      "is_contentious": false
    }
  }'

# Create a chat with both characters
curl -X POST http://localhost:3000/api/v1/chats \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "chat": {
      "name": "Philosophy Discussion",
      "character_slugs": ["sophia", "marcus"]
    }
  }'
```

**Response:**
```json
{
  "chat": {
    "id": "550e8400-e29b-41d4-a716-446655440000",
    "name": "Philosophy Discussion",
    "characters": [
      {
        "slug": "sophia",
        "name": "Sophia",
        ...
      },
      {
        "slug": "marcus",
        "name": "Marcus",
        ...
      }
    ]
  }
}
```

**Save the chat ID for messaging!**

### List Your Chats

```bash
curl -X GET "http://localhost:3000/api/v1/chats?page=1&per_page=20" \
  -H "Authorization: Bearer $TOKEN"
```

### Get a Specific Chat

```bash
CHAT_ID="550e8400-e29b-41d4-a716-446655440000"

curl -X GET http://localhost:3000/api/v1/chats/$CHAT_ID \
  -H "Authorization: Bearer $TOKEN"
```

### Update Chat Name

```bash
curl -X PATCH http://localhost:3000/api/v1/chats/$CHAT_ID \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "chat": {
      "name": "Deep Philosophical Thoughts"
    }
  }'
```

### Delete a Chat

```bash
# Warning: This will also delete all messages in the chat
curl -X DELETE http://localhost:3000/api/v1/chats/$CHAT_ID \
  -H "Authorization: Bearer $TOKEN"
```

## Working with Messages

### Send a Message from User

```bash
CHAT_ID="550e8400-e29b-41d4-a716-446655440000"

curl -X POST http://localhost:3000/api/v1/chats/$CHAT_ID/messages \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "message": {
      "content": "What is the meaning of life?",
      "is_bot": false
    }
  }'
```

### Send a Message from a Character

First, get the character ID from the chat, then:

```bash
CHARACTER_ID=1  # Replace with actual character ID

curl -X POST http://localhost:3000/api/v1/chats/$CHAT_ID/messages \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "message": {
      "content": "That is a profound question! The meaning of life is often considered to be...",
      "character_id": '$CHARACTER_ID',
      "is_bot": true
    }
  }'
```

### List Messages in a Chat

```bash
# Get most recent 50 messages
curl -X GET "http://localhost:3000/api/v1/chats/$CHAT_ID/messages?page=1&per_page=50" \
  -H "Authorization: Bearer $TOKEN"

# Get older messages (page 2)
curl -X GET "http://localhost:3000/api/v1/chats/$CHAT_ID/messages?page=2&per_page=50" \
  -H "Authorization: Bearer $TOKEN"
```

## Complete Example: Full Conversation Flow

```bash
#!/bin/bash

# Configuration
API_URL="http://localhost:3000/api/v1"

# 1. Register user
echo "Registering user..."
RESPONSE=$(curl -s -X POST $API_URL/users/register \
  -H "Content-Type: application/json" \
  -d '{
    "user": {
      "email": "demo@example.com",
      "password": "DemoPassword123",
      "password_confirmation": "DemoPassword123"
    }
  }')

TOKEN=$(echo $RESPONSE | jq -r '.token')
echo "Got token: ${TOKEN:0:20}..."

# 2. Create a character
echo "Creating character..."
CHAR_RESPONSE=$(curl -s -X POST $API_URL/characters \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "character": {
      "name": "Echo",
      "description": "A simple echo bot for testing",
      "greeting": "Hello! I will echo your messages.",
      "persona": "I am a simple echo bot.",
      "visibility": "public",
      "is_contentious": false
    }
  }')

CHAR_SLUG=$(echo $CHAR_RESPONSE | jq -r '.character.slug')
CHAR_ID=$(echo $CHAR_RESPONSE | jq -r '.character.id')
echo "Created character: $CHAR_SLUG (ID: $CHAR_ID)"

# 3. Create a chat
echo "Creating chat..."
CHAT_RESPONSE=$(curl -s -X POST $API_URL/chats \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "chat": {
      "name": "Test Chat",
      "character_slugs": ["'$CHAR_SLUG'"]
    }
  }')

CHAT_ID=$(echo $CHAT_RESPONSE | jq -r '.chat.id')
echo "Created chat: $CHAT_ID"

# 4. Send a message
echo "Sending user message..."
curl -s -X POST $API_URL/chats/$CHAT_ID/messages \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "message": {
      "content": "Hello, Echo!",
      "is_bot": false
    }
  }' | jq

# 5. Send character response
echo "Sending character message..."
curl -s -X POST $API_URL/chats/$CHAT_ID/messages \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "message": {
      "content": "Hello, Echo!",
      "character_id": '$CHAR_ID',
      "is_bot": true
    }
  }' | jq

# 6. List messages
echo "Listing messages..."
curl -s -X GET "$API_URL/chats/$CHAT_ID/messages?page=1&per_page=10" \
  -H "Authorization: Bearer $TOKEN" | jq

echo "Done!"
```

## Error Handling Examples

### Invalid Authentication

```bash
curl -X GET http://localhost:3000/api/v1/chats \
  -H "Authorization: Bearer invalid_token"
```

**Response (401):**
```json
{
  "message": "Not authenticated.",
  "details": "The provided authentication token was incorrect.",
  "param": null
}
```

### Validation Error

```bash
curl -X POST http://localhost:3000/api/v1/characters \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "character": {
      "name": "A",
      "description": "Too short",
      "greeting": "Hi",
      "persona": "Too short"
    }
  }'
```

**Response (400):**
```json
{
  "message": "Description is too short",
  "param": "description",
  "details": "..."
}
```

### Unauthorized Access

```bash
# Try to update another user's character
curl -X PATCH http://localhost:3000/api/v1/characters/someone-elses-character \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"character": {"name": "Hacked"}}'
```

**Response (401):**
```json
{
  "message": "You're not authorized to perform that action."
}
```

## Testing with Python

```python
import requests
import json

API_URL = "http://localhost:3000/api/v1"

# Register
response = requests.post(f"{API_URL}/users/register", json={
    "user": {
        "email": "python@example.com",
        "password": "TestPassword123",
        "password_confirmation": "TestPassword123"
    }
})
token = response.json()["token"]
headers = {"Authorization": f"Bearer {token}"}

# Create character
character = requests.post(f"{API_URL}/characters", headers=headers, json={
    "character": {
        "name": "PyBot",
        "description": "A Python-created bot",
        "greeting": "Hello from Python!",
        "persona": "I am a bot created via Python script.",
        "visibility": "public",
        "is_contentious": False
    }
}).json()

print(f"Created character: {character['character']['slug']}")

# Create chat
chat = requests.post(f"{API_URL}/chats", headers=headers, json={
    "chat": {
        "name": "Python Test Chat",
        "character_slugs": [character["character"]["slug"]]
    }
}).json()

chat_id = chat["chat"]["id"]
print(f"Created chat: {chat_id}")

# Send message
message = requests.post(
    f"{API_URL}/chats/{chat_id}/messages",
    headers=headers,
    json={"message": {"content": "Hello from Python!", "is_bot": False}}
).json()

print(f"Sent message: {message['message']['id']}")

# List messages
messages = requests.get(
    f"{API_URL}/chats/{chat_id}/messages",
    headers=headers
).json()

print(f"Total messages: {len(messages['messages'])}")
```

## Testing with JavaScript/Node.js

```javascript
const axios = require('axios');

const API_URL = 'http://localhost:3000/api/v1';

async function main() {
  // Register
  const { data: { token } } = await axios.post(`${API_URL}/users/register`, {
    user: {
      email: 'js@example.com',
      password: 'TestPassword123',
      password_confirmation: 'TestPassword123'
    }
  });

  const headers = { Authorization: `Bearer ${token}` };

  // Create character
  const { data: { character } } = await axios.post(
    `${API_URL}/characters`,
    {
      character: {
        name: 'JSBot',
        description: 'A JavaScript-created bot',
        greeting: 'Hello from JavaScript!',
        persona: 'I am a bot created via JavaScript.',
        visibility: 'public',
        is_contentious: false
      }
    },
    { headers }
  );

  console.log(`Created character: ${character.slug}`);

  // Create chat
  const { data: { chat } } = await axios.post(
    `${API_URL}/chats`,
    {
      chat: {
        name: 'JS Test Chat',
        character_slugs: [character.slug]
      }
    },
    { headers }
  );

  console.log(`Created chat: ${chat.id}`);

  // Send message
  await axios.post(
    `${API_URL}/chats/${chat.id}/messages`,
    {
      message: {
        content: 'Hello from JavaScript!',
        is_bot: false
      }
    },
    { headers }
  );

  // List messages
  const { data: { messages } } = await axios.get(
    `${API_URL}/chats/${chat.id}/messages`,
    { headers }
  );

  console.log(`Total messages: ${messages.length}`);
}

main().catch(console.error);
```

## Tips

1. **Save your token**: Store it securely for subsequent requests
2. **Check response codes**: 200/201 for success, 400 for validation, 401 for auth, 404 for not found
3. **Use pagination**: Don't try to load all items at once
4. **Character IDs vs Slugs**: Use slugs for user-friendly URLs, IDs for internal references
5. **Chat IDs are UUIDs**: They're longer than integer IDs, plan accordingly
6. **Messages are ordered newest first**: Page through them to get history

## Common Workflows

### Multi-Character Roleplay
1. Create multiple characters with distinct personalities
2. Create a chat with all characters
3. Send messages alternating between characters
4. Use `character_id` to specify who's speaking

### Character Discovery
1. List public characters
2. Filter by contentious status
3. View character details
4. Create chat with selected characters

### Chat Management
1. List all your chats
2. View specific chat with participants
3. Send/receive messages
4. Update chat name as conversation evolves
5. Delete when done

