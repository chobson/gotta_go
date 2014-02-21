class Sensor < ActiveRecord::Base
  has_many :activities
  belongs_to :restroom

  validates :name, presence: true
  validates :token, presence: true
  validates :restroom_id, presence: true
  validates :active, presence: true

  default_scope { order(:active) }

  def occupied
    if self.activities.active.size == 0
      self.activities.create!({start: Time.now})
    end
  end

  def available
    if self.activities.active.size == 1
      activity = self.activities.active.first
      activity.stop = Time.now
      activity.save
    end
  end
end
