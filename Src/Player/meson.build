project('player', 'c')

deps = [
  dependency('libvlc', version: '>=3.0.8'),
  dependency('ncurses', version: '>=6.1')
]

srcs = [
  'player.c',
  'test.c'
]

executable('player', sources: srcs, dependencies: deps)
