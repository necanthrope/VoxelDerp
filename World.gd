extends Spatial
tool

#https://www.youtube.com/watch?v=Q2iWDNq5PaU

var chunk_scene = preload("res://CubicChunk.tscn")

var load_radius = 5
onready var chunks = $Chunks
onready var player = $Player

var load_thread = Thread.new()

func _ready():
	for i in range(0, load_radius):
		for j in range(0, load_radius):
			for k in range(0, load_radius):
				var chunk = chunk_scene.instance()
				var player_chunk_coords = _get_player_chunk_coords(Vector3(i, j, k))
				
				chunk.set_chunk_position(player_chunk_coords)
				chunks.add_child(chunk)
	
	load_thread.start(self, "_thread_process", null)
	
	player.connect("place_block", self, "_on_Player_place_block")
	player.connect("break_block", self, "_on_Player_break_block")

func _thread_process(_userdata):
	while(true):
		for c in chunks.get_children():
			var cx = c.chunk_position.x
			var cy = c.chunk_position.y
			var cz = c.chunk_position.z
			
			var player_chunk_coords = _get_player_chunk_coords(c.chunk_position)
			var new_x = player_chunk_coords.x
			var new_y = player_chunk_coords.y
			var new_z = player_chunk_coords.z
			
			if (new_x != cx or new_y != cy or new_z != cz):
				c.set_chunk_position(Vector3(int(new_x), int(new_y), int(new_z)))
				c.generate()
				c.update()

# Fix this with a hash lookup by chunk coordinates
func get_chunk(chunk_pos):
	for c in chunks.get_children():
		if c.chunk_position == chunk_pos:
			return c
	return null

func _on_Player_place_block(pos, t):
	var cx = int(floor(pos.x / Global.DIMENSION.x))
	var cy = int(floor(pos.y / Global.DIMENSION.y))
	var cz = int(floor(pos.z / Global.DIMENSION.z))
	
	var bx = posmod(floor(pos.x), Global.DIMENSION.x)
	var by = posmod(floor(pos.y), Global.DIMENSION.y)
	var bz = posmod(floor(pos.z), Global.DIMENSION.z)
	
	var c = get_chunk(Vector3(cx, cy, cz))
	if c != null:
		c.blocks[bx][by][bz] = t
		c.update()

func _on_Player_break_block(pos):
	_on_Player_place_block(pos, Global.AIR)
	
func _get_player_chunk_coords(chunk_position):
	
	var cx = chunk_position.x
	var cy = chunk_position.y
	var cz = chunk_position.z
		
	var px = floor(player.translation.x / Global.DIMENSION.x)
	var py = floor(player.translation.y / Global.DIMENSION.y)
	var pz = floor(player.translation.z / Global.DIMENSION.z)
	
	# offset by chunk position player is in, this will be 0 at game start
	var new_x = posmod(cx - px + load_radius/2, load_radius) + px - load_radius/2
	var new_y = posmod(cy - py + load_radius/2, load_radius) + py - load_radius/2
	var new_z = posmod(cz - pz + load_radius/2, load_radius) + pz - load_radius/2
	
	var chunk_coords = Vector3(new_x, new_y, new_z)
	return chunk_coords