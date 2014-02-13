class CreateRestrooms < ActiveRecord::Migration
  def change
    create_table :restrooms do |t|
      t.string :name

      t.timestamps
    end
  end
end
