# API Routes Reference

Complete list of all available routes in the Paphos Backend API.

**Base Path:** `/api/v1`

## Authentication Routes
*These routes do not require authentication*

| Method | Path | Action | Description |
|--------|------|--------|-------------|
| POST | `/users/register` | `Api::V1::Users::Register::Create` | Register a new user account |
| POST | `/users/login` | `Api::V1::Users::Login::Create` | Login and receive JWT token |

## User Routes
*Requires authentication*

| Method | Path | Action | Description |
|--------|------|--------|-------------|
| GET | `/users/me` | `Api::V1::Users::Me::Show` | Get current user profile |

## Character Routes
*Requires authentication*

| Method | Path | Action | Description |
|--------|------|--------|-------------|
| GET | `/characters` | `Api::V1::Characters::Index` | List all visible characters (paginated) |
| GET | `/characters/:character_slug` | `Api::V1::Characters::Show` | Get a specific character by slug |
| POST | `/characters` | `Api::V1::Characters::Create` | Create a new character |
| PATCH | `/characters/:character_slug` | `Api::V1::Characters::Update` | Update a character (owner only) |
| DELETE | `/characters/:character_slug` | `Api::V1::Characters::Delete` | Delete a character (owner only) |

## Chat Routes
*Requires authentication*

| Method | Path | Action | Description |
|--------|------|--------|-------------|
| GET | `/chats` | `Api::V1::Chats::Index` | List all user's chats (paginated) |
| GET | `/chats/:chat_id` | `Api::V1::Chats::Show` | Get a specific chat by ID |
| POST | `/chats` | `Api::V1::Chats::Create` | Create a new chat with characters |
| PATCH | `/chats/:chat_id` | `Api::V1::Chats::Update` | Update chat name (owner only) |
| DELETE | `/chats/:chat_id` | `Api::V1::Chats::Delete` | Delete a chat and all messages (owner only) |

## Message Routes
*Requires authentication*

| Method | Path | Action | Description |
|--------|------|--------|-------------|
| GET | `/chats/:chat_id/messages` | `Api::V1::Messages::Index` | List messages in a chat (paginated, newest first) |
| POST | `/chats/:chat_id/messages` | `Api::V1::Messages::Create` | Send a message in a chat |

## Other Routes

| Method | Path | Action | Description |
|--------|------|--------|-------------|
| GET | `/` | `Home::Index` | Health check / API info |

---

## Route Parameters

### Path Parameters

- `:character_slug` - String: URL-friendly slug (e.g., "my-character")
- `:chat_id` - UUID: Chat identifier (e.g., "550e8400-e29b-41d4-a716-446655440000")

### Query Parameters

#### Pagination (available on index routes)
- `page` - Integer (default: 1): Page number
- `per_page` - Integer (default: 20, max: 100): Items per page

#### Filtering
- `contentious` - Boolean (`true`/`false`): Filter characters by contentious status

### Authentication Parameter
- `auth_token` - String: JWT token (alternative to Authorization header)

---

## Route Constraints

### Character Routes

**Visibility Rules:**
- **Index**: Shows public characters + user's own characters
- **Show**: Shows public/unlisted characters + user's own private characters

**Update Constraints:**
- Must be character owner
- Cannot change to "private" if character is used in other users' chats

**Delete Constraints:**
- Must be character owner
- Cannot delete if character is used in any chats (even own chats)

### Chat Routes

**Access Rules:**
- All operations require chat ownership
- Only chat creator can view, update, or delete

**Delete Behavior:**
- Cascade deletes all messages in the chat
- Does not delete characters (only removes chat_participants entries)

### Message Routes

**Access Rules:**
- Only chat owner can view or create messages

**Creation Constraints:**
- Character must be a participant in the chat
- Content must be 1-4096 characters

---

## HTTP Status Codes

| Code | Meaning | When Used |
|------|---------|-----------|
| 200 | OK | Successful GET, PATCH, DELETE |
| 201 | Created | Successful POST |
| 400 | Bad Request | Validation error |
| 401 | Unauthorized | Missing or invalid auth token |
| 403 | Forbidden | Valid auth but not authorized for this resource |
| 404 | Not Found | Resource doesn't exist |
| 500 | Internal Server Error | Unexpected server error |

---

## Response Formats

### Success Response
```json
{
  "resource_name": { ... },
  "page": 1,           // For paginated endpoints
  "per_page": 20       // For paginated endpoints
}
```

### Error Response
```json
{
  "message": "Error message",
  "details": "Additional details (optional)",
  "param": "field_name (optional)"
}
```

---

## Route Generation

To see all routes in your running application:

```bash
$ lucky routes
```

This will display a formatted table of all routes with their HTTP methods, paths, and action classes.

---

## CORS Configuration

By default, CORS is not configured. For production with a frontend:

1. Add the `cors` shard to `shard.yml`
2. Configure CORS middleware in `src/app_server.cr`
3. Specify allowed origins, methods, and headers

Example CORS configuration:

```crystal
# In src/app_server.cr
def middleware : Array(HTTP::Handler)
  [
    Lucky::CorsHandler.new(
      allow_origin: "https://yourdomain.com",
      allow_methods: "GET, POST, PATCH, DELETE",
      allow_headers: "Authorization, Content-Type",
      allow_credentials: true
    ),
    # ... other middleware
  ] of HTTP::Handler
end
```

---

## Rate Limiting

Currently not implemented. Consider adding rate limiting for production:

- Per-IP request limits
- Per-user request limits
- Sliding window or token bucket algorithm

Recommended limits:
- Authentication endpoints: 5 requests/minute
- Read endpoints: 100 requests/minute
- Write endpoints: 20 requests/minute

---

## Nested Routes

Messages are nested under chats:
- `/chats/:chat_id/messages` - Logical grouping
- Ensures messages always have chat context
- Simplifies authorization (chat owner = message access)

Alternative designs considered but not used:
- `/messages?chat_id=...` - Less RESTful
- `/messages/:message_id` - Would need separate authorization logic

---

## Versioning

All routes are under `/api/v1/`:

- Allows future breaking changes under `/api/v2/`
- Clients specify version in URL
- Can maintain multiple versions simultaneously

Future version considerations:
- Breaking changes require new version
- Non-breaking changes can be added to existing version
- Document deprecation timeline for old versions

