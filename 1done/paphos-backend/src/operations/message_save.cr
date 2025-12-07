class SaveMessage < Message::SaveOperation
  needs current_user : User
  needs chat : Chat

  param_key :message

  permit_columns content, character_id, is_bot

  before_save do
    validate_required content
    validate_size_of content, min: 1, max: 4096

    # Ensure the character belongs to this chat if specified
    if character_id.value
      character = CharacterQuery.new.id(character_id.value.not_nil!).first?
      if character.nil?
        character_id.add_error "does not exist"
      else
        # Use Avram query instead of raw SQL
        participant = ChatParticipantQuery.new
          .chat_id(chat.id)
          .character_id(character.id)
          .first?
        if participant.nil?
          character_id.add_error "is not a participant in this chat"
        end
      end
    end

    chat_id.value = chat.id
    user_id.value = current_user.id
  end
end

