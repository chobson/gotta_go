require 'spec_helper'

describe Activity do
  it { should belong_to :sensor }
  it { should validate_presence_of :sensor_id }
  it { should validate_presence_of :start }
end