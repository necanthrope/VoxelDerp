extends Spatial
#tool

#var chunk_scene = preload("res://CubicChunk.tscn")
var chunk_obj = preload("res://bin/CubicChunk.gdns")
var world_obj = preload("res://bin/CubicWorld.gdns")

var load_radius = 5
#var center_plane_chunks = {}
#var stray_chunks = {}
#var _chunks = {}

onready var chunks = $Chunks
onready var player = $Player

var load_thread_center = Thread.new()
var load_thread_stray = Thread.new()

func _ready():
	if not Engine.editor_hint:
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	
	if (chunk_obj == null):
		chunk_obj = load("res://bin/CubicChunk.gdns")
	
	for i in range(0, load_radius):
		for j in range(0, load_radius):
			for k in range(0, load_radius):
				
				#var chunk = chunk_scene.instance()
				var chunk = chunk_obj.new()
				var player_chunk_coords = _get_player_chunk_coords(Vector3(i, j, k))
				
				chunk.set_chunk_position(player_chunk_coords)
				
				#if (j==player_chunk_coords.y):
				#	center_plane_chunks[chunk._chunk_tag] = chunk
				#else:
				#	stray_chunks[chunk._chunk_tag] = chunk
				
				#_chunks[chunk._chunk_tag] = chunk
				chunks.add_child(chunk)
	
	var kids = str(chunks.get_child_count())
	print ("chunk kids: ", kids)
	
	#load_thread_center.start(self, "_thread_process", center_plane_chunks)
	#load_thread_stray.start(self, "_thread_process", stray_chunks)
	load_thread_stray.start(self, "_thread_process")
	
	player.connect("place_block", self, "_on_Player_place_block")
	player.connect("break_block", self, "_on_Player_break_block")
	
	if not Engine.editor_hint:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)

func _thread_process(_userdata):
	while(true):
		for c in chunks.get_children():
			_update_chunk(c)

func _update_chunk(c):
	var cx = c.get_chunk_position().x
	var cy = c.get_chunk_position().y
	var cz = c.get_chunk_position().z
	
	var player_chunk_coords = _get_player_chunk_coords(c.get_chunk_position())
	var new_x = player_chunk_coords.x
	var new_y = player_chunk_coords.y
	var new_z = player_chunk_coords.z
	
	if (new_x != cx or new_y != cy or new_z != cz):
		c.set_chunk_position(Vector3(int(new_x), int(new_y), int(new_z)))
		c.generate()
		c.update()

# Fix this with a hash lookup by chunk coordinates
func get_chunk(chunk_pos):
	var _chunk_tag = (str(chunk_pos.x) + ":" + str(chunk_pos.y) + ":" + str(chunk_pos.z))
	for c in chunks.get_children():
		if c.get_chunk_position() == chunk_pos:
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
		c.blocks_[bx][by][bz] = t
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
