---
description: Subir commits al repositorio usando Conventional Commits 1.0.0
---

# Workflow de Commits - Conventional Commits 1.0.0

## Reglas Generales

- **No pedir permiso** al usuario para hacer commits ni push.
- **Espaciar los commits** aproximadamente 1 hora entre cada uno. No cargar todos de golpe.
- Seguir la especificación **Conventional Commits 1.0.0** al pie de la letra.

## Formato del mensaje de commit

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

## Tipos permitidos

| Tipo       | Uso                                                        |
|------------|------------------------------------------------------------|
| `feat`     | Nueva funcionalidad (correlaciona con MINOR en SemVer)     |
| `fix`      | Corrección de bug (correlaciona con PATCH en SemVer)       |
| `docs`     | Cambios solo en documentación                              |
| `style`    | Formato, punto y coma faltante, etc. (no cambia lógica)    |
| `refactor` | Cambio de código que no corrige bug ni añade feature        |
| `perf`     | Mejora de rendimiento                                      |
| `test`     | Añadir o corregir tests                                    |
| `build`    | Cambios en sistema de build o dependencias                 |
| `chore`    | Tareas de mantenimiento                                    |
| `ci`       | Cambios en configuración de CI                             |

## Breaking Changes

- Usar `!` después del tipo/scope para llamar la atención: `feat!: descripción`
- O usar footer `BREAKING CHANGE: descripción`
- Ambos pueden combinarse.

## Pasos para subir commits

// turbo-all

1. Verificar que los archivos están guardados.

2. Hacer stage de los archivos relevantes al cambio:
```powershell
git add <archivos>
```

3. Crear el commit con mensaje que siga el formato Conventional Commits:
```powershell
git commit -m "<type>[scope]: <description>"
```

4. Hacer push al repositorio:
```powershell
git push
```

5. **Esperar aproximadamente 1 hora** antes del siguiente commit. No enviar todos de golpe.

6. Repetir los pasos 2-5 para cada commit pendiente.

## Ejemplos de commits válidos

```
feat(sucesion): agregar regla de sucesion por edad
fix(csv): corregir parseo cuando hay campos vacios
docs: actualizar README con instrucciones de ejecucion
refactor(arbol): simplificar busqueda de nodos
style: simplificar comentarios en archivos fuente
build: actualizar Makefile para nueva estructura
chore: limpiar archivos temporales
```

## Reglas estrictas

1. El commit DEBE tener prefijo con un tipo (noun), seguido de scope opcional, `!` opcional, y dos puntos + espacio obligatorios.
2. `feat` DEBE usarse cuando se añade una nueva funcionalidad.
3. `fix` DEBE usarse cuando se corrige un bug.
4. El scope es un sustantivo entre paréntesis que describe la sección del código.
5. La descripción DEBE ir inmediatamente después de los dos puntos y espacio.
6. El body DEBE comenzar con una línea en blanco después de la descripción.
7. Los footers DEBEN ir una línea en blanco después del body.
8. `BREAKING CHANGE` DEBE estar en mayúsculas en el footer.
9. Los tipos NO son case-sensitive excepto `BREAKING CHANGE`.
10. `BREAKING-CHANGE` es sinónimo de `BREAKING CHANGE` en footers.
