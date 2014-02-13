class Sensor < ActiveRecord::Base
  has_many :activities
  belongs_to :restroom

  validates :name, presence: true
  validates :token, presence: true
  validates :restroom_id, presence: true
  validates :active, presence: true

  default_scope { order(:active) }
end
