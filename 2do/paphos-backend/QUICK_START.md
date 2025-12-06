# Quick Start Guide

Get Paphos Backend running in 5 minutes!

## Prerequisites

- Crystal 1.7.2+
- PostgreSQL 14+
- Lucky CLI

## Installation

### 1. Install Crystal (if needed)

**macOS:**
```bash
brew install crystal
```

**Linux (Ubuntu/Debian):**
```bash
curl -fsSL https://crystal-lang.org/install.sh | sudo bash
```

**Windows:**
See https://crystal-lang.org/install/

### 2. Install Lucky

```bash
# Install Lucky CLI
brew install luckyframework/lucky/lucky
# or
curl -fsSL https://raw.githubusercontent.com/luckyframework/cli/main/install.sh | bash
```

### 3. Install Dependencies

```bash
cd paphos-backend
shards install
```

## Setup

### Option A: Quick Setup (Docker)

```bash
# Start PostgreSQL and the API server
docker-compose up

# API will be available at http://localhost:5001/api/v1
```

**Done!** Skip to [Testing](#testing) below.

### Option B: Local Setup

#### 1. Start PostgreSQL

```bash
# macOS
brew services start postgresql

# Linux
sudo systemctl start postgresql

# Or via Docker
docker run -d \
  -p 5432:5432 \
  -e POSTGRES_USER=postgres \
  -e POSTGRES_PASSWORD=postgres \
  postgres:14-alpine
```

#### 2. Create Database

```bash
lucky db.create
lucky db.migrate
```

#### 3. Start Server

```bash
lucky dev
```

**API available at:** http://localhost:3000/api/v1

## Testing

### 1. Health Check

```bash
curl http://localhost:3000/api/v1/
```

**Expected response:**
```json
{"hello":"Hello World from Home::Index"}
```

### 2. Register a User

```bash
curl -X POST http://localhost:3000/api/v1/users/register \
  -H "Content-Type: application/json" \
  -d '{
    "user": {
      "email": "test@example.com",
      "password": "password123",
      "password_confirmation": "password123"
    }
  }'
```

**Save the token from the response!**

### 3. Create a Character

```bash
TOKEN="your_token_here"

curl -X POST http://localhost:3000/api/v1/characters \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "character": {
      "name": "Alice",
      "description": "A friendly AI assistant",
      "greeting": "Hello! How can I help you today?",
      "persona": "I am a helpful and friendly assistant.",
      "visibility": "public",
      "is_contentious": false
    }
  }'
```

### 4. List Characters

```bash
curl http://localhost:3000/api/v1/characters \
  -H "Authorization: Bearer $TOKEN"
```

**Success!** You now have a working Paphos Backend.

## Next Steps

### Learn the API

1. **Read the [API Documentation](./API.md)** - Complete endpoint reference
2. **Try the [Examples](./EXAMPLES.md)** - Practical usage examples
3. **Check [Routes](./ROUTES.md)** - All available routes

### Common Tasks

#### Create a Chat

```bash
curl -X POST http://localhost:3000/api/v1/chats \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "chat": {
      "name": "My First Chat",
      "character_slugs": ["alice"]
    }
  }'
```

Save the `chat.id` from the response!

#### Send a Message

```bash
CHAT_ID="your_chat_id_here"

curl -X POST http://localhost:3000/api/v1/chats/$CHAT_ID/messages \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "message": {
      "content": "Hello, Alice!",
      "is_bot": false
    }
  }'
```

#### View Messages

```bash
curl http://localhost:3000/api/v1/chats/$CHAT_ID/messages \
  -H "Authorization: Bearer $TOKEN"
```

## Troubleshooting

### Database Connection Error

```bash
# Check PostgreSQL is running
pg_isready

# Check connection manually
psql -U postgres -h localhost
```

**Fix:** Ensure PostgreSQL is running and credentials match `config/database.cr`

### Port Already in Use

```bash
# Find and kill process on port 3000
lsof -ti:3000 | xargs kill -9
```

### Migration Errors

```bash
# Reset database (WARNING: deletes all data)
lucky db.drop
lucky db.create
lucky db.migrate
```

### Shard Installation Issues

```bash
# Clear cache and reinstall
rm -rf lib/ shard.lock
shards install
```

## Development Tips

### Auto-reload on Changes

```bash
# Use lucky dev instead of manual server start
lucky dev
```

This watches for file changes and reloads automatically.

### View All Routes

```bash
lucky routes
```

### Database Console

```bash
# Connect to development database
psql -U postgres -d paphos_development
```

### Check Migration Status

```bash
lucky db.migrate.status
```

### Generate New Migration

```bash
lucky gen.migration CreateSomething
```

## Configuration

### Default Ports

- **Development:** 3000 (configurable in `config/watch.yml`)
- **Docker:** 5001 (mapped to 3000 inside container)
- **PostgreSQL:** 5432 (6543 in Docker to avoid conflicts)

### Default Database Credentials

- **Username:** postgres
- **Password:** postgres
- **Database:** paphos_development

Change in `config/database.cr` or set environment variables.

### Environment Variables

Create a `.env` file:

```bash
DATABASE_URL=postgres://user:pass@host:5432/dbname
SECRET_KEY_BASE=your_secret_key
PORT=3000
LUCKY_ENV=development
```

## Useful Commands

```bash
# Development server with auto-reload
lucky dev

# Create database
lucky db.create

# Run migrations
lucky db.migrate

# Rollback last migration
lucky db.rollback

# Drop database (warning: deletes all data)
lucky db.drop

# Show all routes
lucky routes

# Generate secret key
lucky gen.secret_key

# Run tests
crystal spec

# Build for production
shards build --production --release
```

## API Quick Reference

### Authentication

```bash
# Register
POST /api/v1/users/register

# Login
POST /api/v1/users/login

# Get current user
GET /api/v1/users/me
```

### Characters

```bash
# List characters
GET /api/v1/characters

# Get character
GET /api/v1/characters/:slug

# Create character
POST /api/v1/characters

# Update character
PATCH /api/v1/characters/:slug

# Delete character
DELETE /api/v1/characters/:slug
```

### Chats

```bash
# List chats
GET /api/v1/chats

# Get chat
GET /api/v1/chats/:id

# Create chat
POST /api/v1/chats

# Update chat
PATCH /api/v1/chats/:id

# Delete chat
DELETE /api/v1/chats/:id
```

### Messages

```bash
# List messages
GET /api/v1/chats/:chat_id/messages

# Send message
POST /api/v1/chats/:chat_id/messages
```

## Getting Help

### Documentation

- **[API.md](./API.md)** - Complete API reference
- **[EXAMPLES.md](./EXAMPLES.md)** - Usage examples
- **[DEPLOYMENT.md](./DEPLOYMENT.md)** - Production deployment
- **[PROJECT_SUMMARY.md](./PROJECT_SUMMARY.md)** - Project overview

### External Resources

- [Lucky Framework Guides](https://luckyframework.org/guides)
- [Crystal Documentation](https://crystal-lang.org/reference/)
- [PostgreSQL Docs](https://www.postgresql.org/docs/)

### Common Issues

**"No such column"** - Run `lucky db.migrate`  
**"Port already in use"** - Kill process or change port in `config/watch.yml`  
**"Connection refused"** - Start PostgreSQL  
**"Invalid token"** - Re-login to get a new token  
**"Not authorized"** - Check you own the resource  

## Next Steps

✅ API is running  
✅ Created your first user, character, chat, and message  

**Now:**

1. **Explore the API** with the [Examples](./EXAMPLES.md)
2. **Build a frontend** using the documented endpoints
3. **Deploy to production** following [DEPLOYMENT.md](./DEPLOYMENT.md)
4. **Add features** from the enhancement list in [PROJECT_SUMMARY.md](./PROJECT_SUMMARY.md)

**Happy coding! 🚀**

