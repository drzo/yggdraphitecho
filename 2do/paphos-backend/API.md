# Paphos Backend API Documentation

## Base URL
- Development: `http://localhost:3000/api/v1`
- Production: `https://your-domain.com/api/v1`

## Authentication

All endpoints (except `/users/register` and `/users/login`) require authentication via JWT token.

**Include token in requests using either:**
- Header: `Authorization: Bearer <token>`
- Query param: `?auth_token=<token>`

---

## API Endpoints

### Authentication

#### Register User
```http
POST /users/register
```

**Request Body:**
```json
{
  "user": {
    "email": "user@example.com",
    "password": "your_password",
    "password_confirmation": "your_password"
  }
}
```

**Response:**
```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
}
```

#### Login
```http
POST /users/login
```

**Request Body:**
```json
{
  "user": {
    "email": "user@example.com",
    "password": "your_password"
  }
}
```

**Response:**
```json
{
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
}
```

### User Profile

#### Get Current User
```http
GET /users/me
```

**Response:**
```json
{
  "user": {
    "email": "user@example.com"
  }
}
```

---

### Characters

#### List Characters
```http
GET /characters?page=1&per_page=20&contentious=false
```

**Query Parameters:**
- `page` (optional): Page number, default: 1
- `per_page` (optional): Items per page (max 100), default: 20
- `contentious` (optional): Filter by contentious status (`true`/`false`)

**Response:**
```json
{
  "characters": [
    {
      "slug": "character-name",
      "name": "Character Name",
      "description": "A brief description",
      "avatar_id": "avatar_123"
    }
  ],
  "page": 1,
  "per_page": 20
}
```

#### Get Character by Slug
```http
GET /characters/:character_slug
```

**Response:**
```json
{
  "character": {
    "slug": "character-name",
    "name": "Character Name",
    "description": "A brief description",
    "avatar_id": "avatar_123",
    "greeting": "Hello! How can I help you today?",
    "persona": "I am a helpful assistant...",
    "world_scenario": "In a modern office setting...",
    "example_chats": "User: Hi\nCharacter: Hello!",
    "visibility": "public",
    "is_contentious": false
  }
}
```

#### Create Character
```http
POST /characters
```

**Request Body:**
```json
{
  "character": {
    "name": "Character Name",
    "description": "A brief description (8-64 chars)",
    "avatar_id": "optional_avatar_id",
    "greeting": "Hello! (2-1024 chars)",
    "persona": "I am a helpful assistant... (12-1024 chars)",
    "world_scenario": "Optional scenario (max 512 chars)",
    "example_chats": "Optional examples (max 1024 chars)",
    "visibility": "public",
    "is_contentious": false
  }
}
```

**Visibility options:** `public`, `unlisted`, `private`

**Response:**
```json
{
  "character": {
    "slug": "character-name",
    "name": "Character Name",
    ...
  }
}
```

#### Update Character
```http
PATCH /characters/:character_slug
```

**Request Body:** (all fields optional)
```json
{
  "character": {
    "name": "Updated Name",
    "description": "Updated description",
    "visibility": "unlisted"
  }
}
```

**Note:** Cannot change visibility to "private" if the character is used in other users' chats.

**Response:**
```json
{
  "character": { ... }
}
```

#### Delete Character
```http
DELETE /characters/:character_slug
```

**Note:** Cannot delete if character is used in any active chats.

**Response:** `200 OK`

---

### Chats

#### List Chats
```http
GET /chats?page=1&per_page=20
```

**Query Parameters:**
- `page` (optional): Page number, default: 1
- `per_page` (optional): Items per page (max 100), default: 20

**Response:**
```json
{
  "chats": [
    {
      "id": "550e8400-e29b-41d4-a716-446655440000",
      "name": "My Chat",
      "characters": [
        {
          "slug": "character-name",
          "name": "Character Name",
          "description": "Description",
          "avatar_id": "avatar_123"
        }
      ]
    }
  ],
  "page": 1,
  "per_page": 20
}
```

#### Get Chat by ID
```http
GET /chats/:chat_id
```

**Response:**
```json
{
  "chat": {
    "id": "550e8400-e29b-41d4-a716-446655440000",
    "name": "My Chat",
    "characters": [ ... ]
  }
}
```

#### Create Chat
```http
POST /chats
```

**Request Body:**
```json
{
  "chat": {
    "name": "My Chat (optional, 1-32 chars)",
    "character_slugs": ["character-1", "character-2"]
  }
}
```

**Response:**
```json
{
  "chat": {
    "id": "550e8400-e29b-41d4-a716-446655440000",
    "name": "My Chat",
    "characters": [ ... ]
  }
}
```

#### Update Chat
```http
PATCH /chats/:chat_id
```

**Request Body:**
```json
{
  "chat": {
    "name": "Updated Chat Name"
  }
}
```

**Response:**
```json
{
  "chat": { ... }
}
```

#### Delete Chat
```http
DELETE /chats/:chat_id
```

**Note:** All messages in the chat will be deleted as well (cascade delete).

**Response:** `200 OK`

---

### Messages

#### List Messages for a Chat
```http
GET /chats/:chat_id/messages?page=1&per_page=50
```

**Query Parameters:**
- `page` (optional): Page number, default: 1
- `per_page` (optional): Items per page (max 100), default: 50

**Response:**
```json
{
  "messages": [
    {
      "id": 123,
      "chat_id": "550e8400-e29b-41d4-a716-446655440000",
      "character_id": 45,
      "user_id": 1,
      "content": "Hello! How are you?",
      "is_bot": false,
      "created_at": 1677123456
    }
  ],
  "page": 1,
  "per_page": 50
}
```

**Note:** Messages are returned in reverse chronological order (newest first).

#### Create Message
```http
POST /chats/:chat_id/messages
```

**Request Body:**
```json
{
  "message": {
    "content": "Hello! (1-4096 chars)",
    "character_id": 45,
    "is_bot": false
  }
}
```

**Fields:**
- `content` (required): The message text
- `character_id` (optional): Which character is speaking (must be a participant in the chat)
- `is_bot` (optional): Whether this is a bot-generated message, default: false

**Response:**
```json
{
  "message": {
    "id": 124,
    "chat_id": "550e8400-e29b-41d4-a716-446655440000",
    "character_id": 45,
    "user_id": 1,
    "content": "Hello!",
    "is_bot": false,
    "created_at": 1677123456
  }
}
```

---

## Error Responses

All errors follow this format:

```json
{
  "message": "Error message",
  "details": "Additional details (optional)",
  "param": "field_name (optional)"
}
```

### Common HTTP Status Codes

- `200` - Success
- `400` - Bad Request (validation error)
- `401` - Unauthorized (missing or invalid auth token)
- `403` - Forbidden (not authorized to access resource)
- `404` - Not Found
- `500` - Internal Server Error

### Example Error Responses

**Validation Error:**
```json
{
  "message": "Content is required",
  "param": "content"
}
```

**Authorization Error:**
```json
{
  "message": "Not authenticated.",
  "details": "An authentication token is required. Please include a token in an 'auth_token' param or 'Authorization' header."
}
```

**Forbidden:**
```json
{
  "message": "You're not authorized to perform that action."
}
```

---

## Data Models

### User
- `id`: Int64
- `email`: String
- `encrypted_password`: String (not exposed via API)
- `created_at`: Time
- `updated_at`: Time

### Character
- `id`: Int64
- `slug`: String (unique, auto-generated from name)
- `name`: String (1-32 chars)
- `description`: String (8-64 chars)
- `avatar_id`: String? (optional)
- `greeting`: String (2-1024 chars)
- `persona`: String (12-1024 chars)
- `world_scenario`: String? (max 512 chars)
- `example_chats`: String? (max 1024 chars)
- `visibility`: String (`public`, `unlisted`, `private`)
- `is_contentious`: Bool
- `creator_id`: Int64
- `created_at`: Time
- `updated_at`: Time

### Chat
- `id`: UUID
- `name`: String? (1-32 chars)
- `creator_id`: Int64
- `created_at`: Time
- `updated_at`: Time

### Message
- `id`: Int64
- `chat_id`: UUID
- `character_id`: Int64? (optional)
- `user_id`: Int64? (optional)
- `content`: String (1-4096 chars)
- `is_bot`: Bool
- `created_at`: Time
- `updated_at`: Time

### ChatParticipant (join table)
- `id`: Int64
- `chat_id`: UUID
- `character_id`: Int64

---

## Character Visibility Levels

- **public**: Visible to all users in character lists
- **unlisted**: Not shown in lists, but accessible via direct link (slug)
- **private**: Only visible to the creator

## Notes

- All timestamps are returned as Unix timestamps (seconds since epoch)
- Pagination is available on all list endpoints
- Characters can only be deleted if not used in any chats
- Characters cannot be made private if they're used in other users' chats
- When a chat is deleted, all its messages are automatically deleted (cascade)

