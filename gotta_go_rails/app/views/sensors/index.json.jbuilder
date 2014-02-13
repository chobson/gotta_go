json.array!(@sensors) do |sensor|
  json.extract! sensor, :id, :name, :token, :restroom_id
  json.url sensor_url(sensor, format: :json)
end
