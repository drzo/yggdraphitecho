class Api::V1::Messages::Create < ApiAction
  post "/chats/:chat_id/messages" do
    chat = ChatQuery.new.find(chat_id)
    
    # Ensure user owns this chat
    if chat.creator_id != current_user.id
      json ErrorSerializer.new(
        message: "You don't have access to this chat"
      ), status: 403
    else
      message = SaveMessage.create!(params, current_user: current_user, chat: chat)
      json({message: MessageSerializer.new(message)})
    end
  end
end

