class Api::V1::Chats::Index < ApiAction
  get "/chats" do
    page = params.get?(:page).try(&.to_i) || 1
    per_page = params.get?(:per_page).try(&.to_i) || 20
    
    # Limit per_page to reasonable values
    per_page = 100 if per_page > 100
    per_page = 1 if per_page < 1
    
    chats = ChatQuery.new
      .preload_characters
      .creator_id(current_user.id)
      .order_by(:updated_at, :desc)
      .limit(per_page)
      .offset((page - 1) * per_page)

    serialized_chats = ChatSerializer.for_collection(chats)
    json({
      chats: serialized_chats,
      page: page,
      per_page: per_page
    })
  end
end
