class Api::V1::Characters::Delete < ApiAction
  include CheckCurrentUser

  delete "/characters/:character_slug" do
    character = CharacterQuery.new.slug(character_slug).first
    ensure_owned_by_current_user!(character)

    # Check if character is used in any chats (including from other users)
    participant_count = ChatParticipantQuery.new
      .character_id(character.id)
      .select_count

    if participant_count > 0
      json ErrorSerializer.new(
        message: "Cannot delete character",
        details: "This character is being used in active chats. Please remove it from all chats first."
      ), status: 400
    else
      DeleteCharacter.delete!(character)
      head :ok
    end
  end
end
