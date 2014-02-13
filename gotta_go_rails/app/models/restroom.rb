class Restroom < ActiveRecord::Base
  has_many :sensors

  validates :name, presence: true

  def available?
    if activities.empty? || activities.active.empty?
      true
    else
      false
    end
  end

  def sensor
    self.sensors.first
  end

  def activities
    sensor.activities
  end
end
