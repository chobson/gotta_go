class Activity < ActiveRecord::Base
  belongs_to :sensor

  validates :sensor_id, presence: true
  validates :start, presence: true

  default_scope { order(:start => :desc) }
  scope :active, -> { where stop: nil }
  scope :on_date, -> (date) { where("start >= ? AND start <= ?", date.beginning_of_day, date.end_of_day) }

end
