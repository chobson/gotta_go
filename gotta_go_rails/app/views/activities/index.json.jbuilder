json.array!(@activities) do |activity|
  json.extract! activity, :id, :sensor_id, :start, :stop
  json.url activity_url(activity, format: :json)
end
