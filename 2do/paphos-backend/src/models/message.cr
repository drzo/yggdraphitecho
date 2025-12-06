class Message < BaseModel
  table do
    belongs_to chat : Chat
    belongs_to character : Character?
    belongs_to user : User?

    column content : String
    column is_bot : Bool
  end
end

