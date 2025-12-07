# Changelog

All notable changes to the Paphos Backend project.

## [Implemented] - 2024

### Added - Core Messaging System
- **Message Model**: Complete message system for chat functionality
  - Database migration for messages table
  - Message model with relationships to chats, characters, and users
  - Support for both user and bot messages
  - Character attribution for messages
  - Message query with pagination and filtering

- **Message API Endpoints**:
  - `POST /api/v1/chats/:chat_id/messages` - Create a new message in a chat
  - `GET /api/v1/chats/:chat_id/messages` - List messages with pagination
  - Messages sorted by newest first
  - Validation: 1-4096 character limit
  - Ensures character is a participant in the chat

### Added - Missing API Endpoints

- **Character Endpoints**:
  - `GET /api/v1/characters/:slug` - Get a specific character by slug
  - Respects visibility rules (accessible_by query)

- **Chat Endpoints**:
  - `GET /api/v1/chats/:chat_id` - Get a specific chat with characters
  - `DELETE /api/v1/chats/:chat_id` - Delete a chat (cascades to messages)
  - Proper ownership validation

- **User Endpoints**:
  - `GET /api/v1/users/me` - Get current authenticated user profile

### Enhanced - Pagination

- Added pagination to character listing:
  - Query parameters: `page`, `per_page` (max 100)
  - Returns pagination metadata in response
  - Filter by `contentious` status

- Added pagination to chat listing:
  - Query parameters: `page`, `per_page` (max 100)
  - Sorted by last update (most recent first)
  - Returns pagination metadata

- Message listing with pagination:
  - Efficient pagination for large message histories
  - Configurable per_page (max 100, default 50)

### Enhanced - Error Handling & Validation

- **Character Deletion Protection**:
  - Cannot delete characters used in any active chats
  - Clear error message with details

- **Character Visibility Protection**:
  - Cannot change character to "private" if used in other users' chats
  - Validates ownership before allowing visibility changes

- **Message Validation**:
  - Ensures character belongs to chat before allowing message creation
  - Content length validation (1-4096 characters)
  - Proper error responses with details

- **Null Handling**:
  - Fixed chat update null string conversion issue
  - Better handling of optional fields

### Enhanced - Database Schema

- **Messages Table**:
  - Composite index on (chat_id, created_at) for efficient querying
  - Cascade delete when chat is deleted
  - Set null when character/user is deleted (preserves message history)

- **Model Relationships**:
  - Added `has_many :messages` to User, Character, and Chat models
  - Proper foreign key constraints

### Added - Queries

- **MessageQuery**: 
  - `for_chat(chat_id)` - Filter by chat
  - `recent_first` - Order by creation date descending
  - `paginated(page, per_page)` - Pagination support

- **ChatParticipantQuery**: Base query for chat participants

### Added - Operations

- **SaveMessage**: Message creation with full validation
- **DeleteChat**: Chat deletion operation

### Added - Serializers

- **MessageSerializer**: JSON serialization for messages
  - Includes all fields with Unix timestamp for created_at
  - Supports collection serialization

### Documentation

- **API.md**: Complete API documentation
  - All endpoints documented with request/response examples
  - Authentication guide
  - Error response formats
  - Data model specifications
  - Character visibility level explanations

- **DEPLOYMENT.md**: Comprehensive deployment guide
  - Development setup instructions
  - Production deployment with Docker
  - Database migration guide
  - Nginx reverse proxy configuration
  - Systemd service setup
  - Security checklist
  - Performance tuning tips
  - Backup strategies

- **Updated README.md**:
  - Project status updated (fully functional)
  - Feature checklist
  - Quick start guide
  - Links to documentation

### Fixed - Code Quality

- Removed all TODOs related to:
  - Pagination implementation ✓
  - Character deletion error handling ✓
  - Admin user type (documented for future)
  - Private character validation ✓

- Improved code consistency:
  - Proper use of Avram queries instead of raw SQL
  - Consistent error handling patterns
  - Proper authorization checks

## Summary of Implementation

### Total Features Implemented:
- ✅ Complete user authentication system (register, login, JWT)
- ✅ Full character CRUD with slug-based URLs
- ✅ Character visibility system (public, unlisted, private)
- ✅ Chat creation and management
- ✅ Multi-character chat participants
- ✅ Complete messaging system
- ✅ Pagination on all list endpoints
- ✅ Comprehensive validation and error handling
- ✅ Full API documentation
- ✅ Deployment guides

### Database Migrations:
1. `00000000000001_create_users.cr` - User accounts
2. `20230223150103_create_characters.cr` - Character profiles
3. `20230223193937_create_chats.cr` - Chat rooms
4. `20230223195245_create_chat_participants.cr` - Chat-Character associations
5. `20230224120000_create_messages.cr` - **NEW** Message system

### API Endpoints Count:
- Authentication: 2 endpoints
- Users: 1 endpoint
- Characters: 5 endpoints (index, show, create, update, delete)
- Chats: 5 endpoints (index, show, create, update, delete)
- Messages: 2 endpoints (index, create)

**Total: 15 REST API endpoints**

### Models:
- User
- Character
- Chat
- ChatParticipant
- Message (NEW)
- UserToken (helper)

### Next Steps (Future Enhancements):
- [ ] WebSocket support for real-time messaging
- [ ] File upload for character avatars
- [ ] Admin user type and permissions
- [ ] Character search/filtering by name
- [ ] Message reactions/editing
- [ ] Chat typing indicators
- [ ] User profiles with avatars
- [ ] Rate limiting middleware
- [ ] API versioning strategy
- [ ] Automated testing suite expansion

