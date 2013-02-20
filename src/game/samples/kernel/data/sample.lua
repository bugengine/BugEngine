kernel = plugin("samples.kernel")

storage = kernel.KernelStorage()
sample = kernel.KernelSampleTask(storage.A, storage.B)
world = BugEngine.World.World(storage, {sample.output})