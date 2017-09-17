#!/bin/bash

curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
sudo apt-get install git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev build-essential nodejs

git clone https://github.com/qemu/qemu.git
cd qemu

git checkout 4cd42653f5c1df326a2678a84f24a78fb9601277
cat << EOF | git apply -
diff --git a/hw/i386/pc_piix.c b/hw/i386/pc_piix.c
index 9f102aa..f6157df 100644
--- a/hw/i386/pc_piix.c
+++ b/hw/i386/pc_piix.c
@@ -300,6 +300,12 @@ static void pc_init1(MachineState *machine,
         nvdimm_init_acpi_state(&pcms->acpi_nvdimm_state, system_io,
                                pcms->fw_cfg, OBJECT(pcms));
     }
+
+    /* Add flag */
+    MemoryRegion *flag;
+    flag = g_malloc(sizeof(*flag));
+    memory_region_init_ram_from_file(flag, NULL, "flag", 0x1000, false, "/opt/funtimejs/flag.txt", &error_fatal);
+    memory_region_add_subregion(system_memory, 0xdeadbee000, flag);
 }
 
 /* Looking for a pc_compat_2_4() function? It doesn't exist.
EOF


./configure --disable-fdt --target-list=x86_64-softmmu
make -j8
sudo make install

sudo npm install runtime-cli -g

sudo mkdir -p /opt/funtimejs
sudo chmod 755 /opt/funtimejs
python -c "f = '\x00'*0xeef; f += 'flag{1_th0t_j@vascript_w@s_mem0ry_s@f3!}'; f += '\x00'*(0x1000-len(f)); print f" | sudo tee /opt/funtimejs/flag.txt >/dev/null
echo "flag{I_f0rg0t_1n1trd_1nclud3d_a11_files}" > /opt/funtimejs/fs_flag.txt
sudo chmod 644 /opt/funtimejs/flag.txt
sudo chmod 644 /opt/funtimejs/fs_flag.txt
