require 'spec_helper'

describe Restroom do
  it { should have_many :sensors }
  it { should validate_presence_of :name }
end