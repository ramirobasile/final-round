project(
	'final-round',
	'cpp',
	version: '0.1',
	license: 'GPLv3',
	default_options : ['cpp_std=c++17']
)

prefix = get_option('prefix')
bindir = get_option('bindir')
datadir =get_option('datadir')
sysconfdir = get_option('sysconfdir')
mandir = get_option('mandir')

subdir('src')
install_subdir('assets', install_dir: join_paths(datadir, 'games', 'final-round'))
install_data('final-round.cfg', install_dir: sysconfdir)
executable('final-round', sources, dependencies: sfmldeps, install_dir: join_paths(bindir, 'games'), install: true)
