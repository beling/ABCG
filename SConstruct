env = Environment().Clone()
debug = int(ARGUMENTS.get('debug', 0))

if env['CXX']=="g++":
	if debug:
		env.Append(CCFLAGS = '-g -Wall -fno-rtti -ansi')
	else:
		if env['CXXVERSION'] >= "4.2.0":
			env.Append(CCFLAGS = '-DNDEBUG -Wall -O3 -pipe -fno-rtti -ansi -march=native -mtune=native' )
		elif env['CXXVERSION'] >= "4.1.0":
			env.Append(CCFLAGS = '-DNDEBUG -Wall -O3 -pipe -fno-rtti -ansi -march=i686 -mtune=generic' )
		else:
			env.Append(CCFLAGS = '-DNDEBUG -Wall -O3 -pipe -fno-rtti -ansi -march=i686 -mtune=i686' )

env.BuildDir('./build', '.', duplicate = 0)
prog = env.Program(
	target = 'build/ABCG',
	source = [Glob('build/*.cpp'), Glob('build/libs/*.cpp')],
	LIBS=['X11', 'Xi', 'Xmu', 'glut', 'GL', 'GLU', 'm']
)

#source = Split("""
#	camera2d.cpp  main.cpp  terrain.cpp  train.cpp    world.cpp
#	link.cpp      node.cpp  timer.cpp    vicicle.cpp
#"""),

#if not debug:
#	env.AddPostAction(prog, Action([['strip','$TARGET']]))
#	env.AddPostAction(prog, Action([['upx', '-q', '$TARGET']]))
