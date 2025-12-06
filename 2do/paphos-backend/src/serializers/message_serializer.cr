class MessageSerializer < BaseSerializer
  def initialize(@message : Message)
  end

  def render
    {
      id:           @message.id,
      chat_id:      @message.chat_id,
      character_id: @message.character_id,
      user_id:      @message.user_id,
      content:      @message.content,
      is_bot:       @message.is_bot,
      created_at:   @message.created_at.to_unix,
    }
  end
end

