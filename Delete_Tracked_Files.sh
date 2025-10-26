# Targets a eliminar
TARGETS=(
    "MetalGear/2DGame/MetalGear/.vs/"
    "MetalGear/2DGame/MetalGear/Debug/MetalGear.exe"
    "MetalGear/2DGame/MetalGear/Debug/MetalGear.pdb"
    "MetalGear/2DGame/MetalGear/MetalGear/Debug/"
)

echo "********************************"
echo "--------------------------------"
echo "Eliminando archivos del directorio de trabajo ..."
echo "--------------------------------"

for pattern in "${PATTERNS[@]}"; do
    if [ -e "$pattern" ]; then
        echo "Eliminando: $pattern"
        rm -rf "$pattern"
    fi
done

echo "--------------------------------"
echo "Eliminando archivos del historial de Git ....."
echo "--------------------------------"
CURRENT_BRANCH=$(git branch --show-current)
echo "Creando backup de $CURRENT_BRANCH -> ${CURRENT_BRANCH}_backup"
git branch ${CURRENT_BRANCH}_backup

for pattern in "${PATTERNS[@]}"; do
    echo "Filtrando del historial: $pattern"
    git filter-branch --force --index-filter \
        "git rm -rf --cached --ignore-unmatch '$pattern'" \
        --prune-empty --tag-name-filter cat -- --all 2>/dev/null
done

echo "--------------------------------"
echo "Limpiando referencias ..."
echo "--------------------------------"

rm -rf .git/refs/original/
git reflog expire --expire=now --all
git gc --prune=now --aggressive

echo "********************************"