class CreateSensors < ActiveRecord::Migration
  def change
    create_table :sensors do |t|
      t.string :name
      t.string :token
      t.integer :restroom_id
      t.boolean :active, default: true

      t.timestamps
    end
  end
end
