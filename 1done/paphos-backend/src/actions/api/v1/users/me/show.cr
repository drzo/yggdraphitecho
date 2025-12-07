class Api::V1::Users::Me::Show < ApiAction
  get "/users/me" do
    json({user: UserSerializer.new(current_user)})
  end
end

