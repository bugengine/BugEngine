kernel = plugin("samples.kernel")
_3d = plugin("graphics.3d")

tokens={}
BugEngine.help(BugEngine.World)

storage = kernel.KernelStorage()
sample = kernel.KernelSampleTask(storage.A, storage.B)
tokens['sample'] = resources:load(sample)
world = BugEngine.World.World(storage, {sample.output})
tokens['world'] = resources:load(world)

e1 = world:spawn()
e2 = world:spawn()
e3 = world:spawn()
e4 = world:spawn()
e5 = world:spawn()
e6 = world:spawn()
e7 = world:spawn()


world:addComponent(e1, kernel.A({value = 1}))


world:unspawn(e1)
world:unspawn(e2)
world:unspawn(e3)
world:unspawn(e4)
world:unspawn(e5)
world:unspawn(e6)
world:unspawn(e7)