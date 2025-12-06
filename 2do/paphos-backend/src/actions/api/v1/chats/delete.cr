class Api::V1::Chats::Delete < ApiAction
  include CheckCurrentUser

  delete "/chats/:chat_id" do
    chat = ChatQuery.new.find(chat_id)
    ensure_owned_by_current_user!(chat)

    # Delete the chat (messages will cascade delete due to on_delete: :cascade)
    ChatQuery.new.id(chat.id).delete

    head :ok
  end
end

