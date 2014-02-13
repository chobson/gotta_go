class Activity < ActiveRecord::Base
  belongs_to :sensor

  validates :sensor_id, presence: true
  validates :start, presence: true

  default_scope { order(:start => :desc) }
  scope :active, -> { where stop: nil }
end
