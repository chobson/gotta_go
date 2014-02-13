json.array!(@restrooms) do |restroom|
  json.extract! restroom, :id, :name
  json.url restroom_url(restroom, format: :json)
end
