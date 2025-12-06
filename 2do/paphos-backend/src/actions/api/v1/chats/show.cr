class Api::V1::Chats::Show < ApiAction
  include CheckCurrentUser

  get "/chats/:chat_id" do
    chat = ChatQuery.new.preload_characters.find(chat_id)
    ensure_owned_by_current_user!(chat)

    json({chat: ChatSerializer.new(chat)})
  end
end

