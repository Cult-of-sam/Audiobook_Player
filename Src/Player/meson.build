project('player', 'c')

deps = [
  dependency('libvlc', version: '>=3.0.8'),
  dependency('ncurses', version: '>=6.1'),
  dependency('threads')
]

srcs = [
  'player.c',
  'player1.c',
  'log.c'
]

executable('player', sources: srcs, dependencies: deps)
