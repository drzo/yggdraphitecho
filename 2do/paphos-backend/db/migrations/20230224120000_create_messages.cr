class CreateMessages::V20230224120000 < Avram::Migrator::Migration::V1
  def migrate
    create table_for(Message) do
      primary_key id : Int64
      add_belongs_to chat : Chat, on_delete: :cascade, foreign_key_type: UUID
      add_belongs_to character : Character?, on_delete: :set_null
      add_belongs_to user : User?, on_delete: :set_null

      add content : String
      add is_bot : Bool, default: false

      add_timestamps
    end

    create_index :messages, [:chat_id, :created_at]
  end

  def rollback
    drop table_for(Message)
  end
end

