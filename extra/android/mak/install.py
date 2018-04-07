from waflib import TaskGen, Utils, Errors
from waflib.TaskGen import taskgen_method
import os

original_install_files = TaskGen.task_gen.install_files
original_install_as = TaskGen.task_gen.install_as


@taskgen_method
def get_package_task(self):
    return self.bld.android_package_task


@taskgen_method
def install_files(self, out_dir, file_list, chmod=Utils.O644, original_install=False):
    if 'android' in self.env.VALID_PLATFORMS:
        if not isinstance(file_list, list):
            file_list = [file_list]
        package_task = self.get_package_task()
        root_path = os.path.join(self.env.PREFIX, self.bld.__class__.optim)
        if original_install or not out_dir.startswith(root_path):
            return original_install_files(self, out_dir, file_list, chmod)
        out_dir = out_dir[len(root_path)+1:]
        for file in file_list:
            filename = file.path_from(self.bld.bldnode)
            base = os.path.dirname(filename)
            if base != out_dir:
                dest_node = self.bld.bldnode
                dest_node = dest_node.make_node(self.bld.bugengine_variant)
                dest_node = dest_node.make_node(self.bld.optim)
                dest_node = dest_node.make_node('zip')
                dest_node = dest_node.make_node(out_dir)
                dest_node.mkdir()
                dest_node = dest_node.find_or_declare(file.name)
                package_task.generator.create_task('copy', [file], [dest_node])
            else:
                dest_node = file
            package_task.set_inputs([dest_node])
    else:
        return original_install_files(self, out_dir, file_list, chmod)


@taskgen_method
def install_as(self, target_path, file, chmod=Utils.O644):
    if 'android' in self.env.VALID_PLATFORMS:
        package_task = self.get_package_task()
        root_path = os.path.join(self.env.PREFIX, self.bld.__class__.optim)
        if not target_path.startswith(root_path):
            raise Errors.WafError('Does not know how to deploy to %s'%target_path)
        dest_node = self.bld.bldnode
        dest_node = dest_node.make_node(self.bld.bugengine_variant)
        dest_node = dest_node.make_node(self.bld.optim)
        dest_node = dest_node.make_node('zip')
        dest_node = dest_node.find_or_declare(target_path[len(root_path)+1:])
        package_task.generator.create_task('copy', [file], [dest_node])
        package_task.set_inputs([dest_node])
    else:
        return original_install_as(self, target_path, file, chmod)


def build(bld):
    pass
