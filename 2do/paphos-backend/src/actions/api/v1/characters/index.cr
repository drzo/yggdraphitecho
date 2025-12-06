class Api::V1::Characters::Index < ApiAction
  get "/characters" do
    page = params.get?(:page).try(&.to_i) || 1
    per_page = params.get?(:per_page).try(&.to_i) || 20
    
    # Limit per_page to reasonable values
    per_page = 100 if per_page > 100
    per_page = 1 if per_page < 1
    
    query = CharacterQuery.new.visible_to(current_user)
    
    # Filter by contentious status if requested
    if params.get?(:contentious) == "true"
      query = query.is_contentious(true)
    elsif params.get?(:contentious) == "false"
      query = query.is_contentious(false)
    end
    
    # Apply pagination
    characters = query
      .limit(per_page)
      .offset((page - 1) * per_page)
    
    serialized_characters = MinimalCharacterSerializer.for_collection(characters)
    json({
      characters: serialized_characters,
      page: page,
      per_page: per_page
    })
  end
end
