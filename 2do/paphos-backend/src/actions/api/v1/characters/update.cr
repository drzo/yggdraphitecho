class Api::V1::Characters::Update < ApiAction
  include CheckCurrentUser

  patch "/characters/:character_slug" do
    character = CharacterQuery.new.slug(character_slug).first
    ensure_owned_by_current_user!(character)

    # Check if trying to make private when used in other users' chats
    if params.get?(:character).try(&.[:visibility]?) == "private"
      # Check if character is used in any chats from other users
      participant_count = ChatParticipantQuery.new
        .character_id(character.id)
        .join_chat
        .where_chats { |chats| chats.creator_id.not.eq(current_user.id) }
        .select_count

      if participant_count > 0
        json ErrorSerializer.new(
          message: "Cannot make character private",
          details: "This character is being used in chats by other users"
        ), status: 400
        return
      end
    end

    updated_character = SaveCharacter.update!(
      character, params, current_user: current_user)

    json({character: FullCharacterSerializer.new(updated_character)})
  end
end
