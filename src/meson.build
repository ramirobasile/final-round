sources = files(
	'config.cpp',
	'dodge.cpp',
	'input.cpp',
	'level.cpp',
	'main.cpp',
	'match.cpp',
	'physics.cpp',
	'player.cpp',
	'punch.cpp',
	'sprite.cpp',
	'state.cpp',
)

sfmldeps = [dependency('sfml-all')]

executable('final-round', sources, dependencies: sfmldeps, install: true, install_dir: join_paths(bindir, 'games'))
