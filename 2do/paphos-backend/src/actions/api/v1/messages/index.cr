class Api::V1::Messages::Index < ApiAction
  get "/chats/:chat_id/messages" do
    chat = ChatQuery.new.find(chat_id)
    
    # Ensure user owns this chat
    if chat.creator_id != current_user.id
      json ErrorSerializer.new(
        message: "You don't have access to this chat"
      ), status: 403
    else
      page = params.get?(:page).try(&.to_i) || 1
      per_page = params.get?(:per_page).try(&.to_i) || 50
      
      # Limit per_page to reasonable values
      per_page = 100 if per_page > 100
      per_page = 1 if per_page < 1
      
      messages = MessageQuery.new
        .for_chat(chat_id)
        .recent_first
        .paginated(page, per_page)
      
      serialized_messages = MessageSerializer.for_collection(messages)
      json({
        messages: serialized_messages,
        page: page,
        per_page: per_page
      })
    end
  end
end

