kernel = plugin("samples.kernel")
_3d = plugin("graphics.3d")

tokens={}
BugEngine.help(BugEngine.World)

storage = kernel.KernelStorage()
sample = kernel.KernelSampleTask(storage.A, storage.B)
tokens['sample'] = resources:load(sample)
world = BugEngine.World.World(storage, {sample.output})
tokens['world'] = resources:load(world)
