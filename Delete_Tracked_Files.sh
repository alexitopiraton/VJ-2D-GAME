# Targets a eliminar
TARGETS=(
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/CopilotIndices/17.14.1231.31060/CodeChunks.db"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/CopilotIndices/17.14.1231.31060/SemanticSymbols.db"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/FileContentIndex/fd86ce1b-c20f-4396-9f11-43f15f05f857.vsidx"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/v17/.suo"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/v17/Browse.VC.db"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/v17/DocumentLayout.backup.json"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/v17/DocumentLayout.json"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/v17/Solution.VC.db"
    "MetalGear/2DGame/MetalGear/.vs/MetalGear/v17/ipch/AutoPCH/b5bb02b91d16e3d3/PLAYER.ipch"
    "MetalGear/2DGame/MetalGear/Debug/MetalGear.exe"
    "MetalGear/2DGame/MetalGear/Debug/MetalGear.pdb"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/Game.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/Map.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.exe.recipe"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.ilk"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.log"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/CL.command.1.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/CL.read.1.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/CL.write.1.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/Cl.items.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/MetalGear.lastbuildstate"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/link.command.1.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/link.read.1.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/link.secondary.1.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/MetalGear.tlog/link.write.1.tlog"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/Player.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/Scene.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/Shader.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/ShaderProgram.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/Sprite.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/Texture.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/TileMap.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/main.obj"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/vc143.idb"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/vc143.pdb"
)

echo "********************************"
echo "--------------------------------"
echo "Limpiando archivos targets ....."
echo "--------------------------------"

for target in "${TARGETS[@]}"; do
    git rm --cached $target 2>/dev/null
done

echo "--------------------------------"
echo "Archivos limpiados"
echo "--------------------------------"
echo "Commit de los nuevos cambios"
echo "--------------------------------"

git commit -m "deleted tracked files"

echo "********************************"