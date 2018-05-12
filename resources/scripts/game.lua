-- game.lua -> for now, handles the game loop

-- DEMO GAME: Breakout



loop = true

-- testing out Lua function calls from C++
function update () 
  -- update stuff
  return "update Func From Lua!"
end

function render () 
  -- return render data stuff (like a scene graph)
  -- to pass to the renderer
  return "render Func from Lua!"
end