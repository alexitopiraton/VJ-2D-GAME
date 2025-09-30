# Targets a eliminar
TARGETS=(
    "MetalGear/2DGame/MetalGear/.vs/"
    "MetalGear/2DGame/MetalGear/Debug/MetalGear.exe"
    "MetalGear/2DGame/MetalGear/Debug/MetalGear.pdb"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/"
)

echo "********************************"
echo "--------------------------------"
echo "Limpiando archivos targets ....."
echo "--------------------------------"

for target in "${TARGETS[@]}"; do
    git rm --cached -r $target 2>/dev/null
done

echo "--------------------------------"
echo "Commit de los nuevos cambios ..."
echo "--------------------------------"

git commit -m "deleted tracked files"

echo "********************************"