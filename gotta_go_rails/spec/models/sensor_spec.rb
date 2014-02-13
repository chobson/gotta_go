require 'spec_helper'

describe Sensor do
  it { should have_many :activities }
  it { should belong_to :restroom }
  it { should validate_presence_of :name }
  it { should validate_presence_of :token }
  it { should validate_presence_of :restroom_id }
  it { should validate_presence_of :active }
end