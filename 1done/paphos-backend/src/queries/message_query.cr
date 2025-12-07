class MessageQuery < Message::BaseQuery
  def for_chat(chat_id : UUID)
    where(&.chat_id(chat_id))
  end

  def recent_first
    order_by(:created_at, :desc)
  end

  def paginated(page : Int32, per_page : Int32 = 50)
    limit(per_page).offset((page - 1) * per_page)
  end
end

