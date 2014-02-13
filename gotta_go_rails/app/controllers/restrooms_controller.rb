class RestroomsController < ApplicationController
  before_action :set_restroom, only: [:show, :edit, :update, :destroy]

  # GET /restrooms
  # GET /restrooms.json
  def index
    @restrooms = Restroom.all
  end

  # GET /restrooms/1
  # GET /restrooms/1.json
  def show
  end

  # GET /restrooms/new
  def new
    @restroom = Restroom.new
  end

  # GET /restrooms/1/edit
  def edit
  end

  # POST /restrooms
  # POST /restrooms.json
  def create
    @restroom = Restroom.new(restroom_params)

    respond_to do |format|
      if @restroom.save
        format.html { redirect_to @restroom, notice: 'Restroom was successfully created.' }
        format.json { render action: 'show', status: :created, location: @restroom }
      else
        format.html { render action: 'new' }
        format.json { render json: @restroom.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /restrooms/1
  # PATCH/PUT /restrooms/1.json
  def update
    respond_to do |format|
      if @restroom.update(restroom_params)
        format.html { redirect_to @restroom, notice: 'Restroom was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: 'edit' }
        format.json { render json: @restroom.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /restrooms/1
  # DELETE /restrooms/1.json
  def destroy
    @restroom.destroy
    respond_to do |format|
      format.html { redirect_to restrooms_url }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_restroom
      @restroom = Restroom.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def restroom_params
      params.require(:restroom).permit(:name)
    end
end
