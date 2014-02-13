class CreateActivities < ActiveRecord::Migration
  def change
    create_table :activities do |t|
      t.integer :sensor_id
      t.datetime :start
      t.datetime :stop

      t.timestamps
    end
  end
end
