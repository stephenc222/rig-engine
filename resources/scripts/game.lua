-- game.lua -> for now, handles the game loop

-- DEMO GAME: Breakout

-- testing out Lua function calls from C++
function update () 
  -- update stuff
  -- TODO: WIP testing out getting button state
  isDown = api:getButtonState(SDL_SCANCODE_A)

  -- TODO: WIP testing out getting stick state
  stick_0_x, stick_0_y = api:getStickState(0)
  -- print("isDown:", isDown)
  -- print("stick_0_x", stick_0_x, "stick_0_y", stick_0_y)
  if isDown == 1 then
    -- print('it is down!') 
  end
  return "update Func From Lua!"
end

function render () 
  -- return render data stuff (like a scene graph)
  -- to pass to the renderer
  return "render Func from Lua!"
end