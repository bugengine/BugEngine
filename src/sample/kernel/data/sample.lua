kernel = plugin("sample.kernel")
_3d = plugin("plugin.graphics.3d")

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
world:addComponent(e1, kernel.B({value = 1}))
world:addComponent(e2, kernel.A({value = 1}))
world:addComponent(e2, kernel.B({value = 1}))
world:addComponent(e3, kernel.A({value = 1}))
world:addComponent(e3, kernel.B({value = 1}))
world:addComponent(e4, kernel.A({value = 1}))
world:addComponent(e4, kernel.B({value = 1}))
world:addComponent(e5, kernel.A({value = 1}))
world:addComponent(e5, kernel.B({value = 1}))
world:addComponent(e6, kernel.A({value = 1}))
world:addComponent(e7, kernel.A({value = 1}))
world:addComponent(e7, kernel.B({value = 1}))

world:removeComponent(e7, kernel.A)

print(world:hasComponent(e7, kernel.A))
print(world:hasComponent(e7, kernel.B))

world:unspawn(e1)
world:unspawn(e2)
world:unspawn(e3)
world:unspawn(e4)
world:unspawn(e5)
world:unspawn(e6)
world:unspawn(e7)
