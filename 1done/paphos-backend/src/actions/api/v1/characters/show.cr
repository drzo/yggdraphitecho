class Api::V1::Characters::Show < ApiAction
  get "/characters/:character_slug" do
    character = CharacterQuery.new
      .accessible_by(current_user)
      .slug(character_slug)
      .first
    
    json({character: FullCharacterSerializer.new(character)})
  end
end

