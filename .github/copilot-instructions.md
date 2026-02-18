# Copilot Instructions for Vibe Tracing

This file provides context for AI coding assistants working on Vibe Tracing, a modular C-based ray tracing engine built in one day via AI-assisted development.

## Project Context

**Vibe Tracing** is a path tracer written in pure C that implements ray-object intersection, materials (Lambertian, Metal, Dielectric), and global illumination. The project demonstrates **vibe coding**—effectively collaborating with AI agents under strict time and token constraints. For full details, see `README.md` and `Specification.md`.

## Build & Test Commands

### Build
```bash
make                 # Build the project (creates executable in current or output directory)
make clean           # Remove build artifacts
```

Note: The Makefile will be created during Step 2 of development. Flags: `-Wall -Wextra -pedantic` with GCC.

### Testing
```bash
# Unit tests (once created in tests/ directory)
make test            # Run all unit tests
tests/test_vec3      # Run individual test executable

# Manual visual validation
./vibe_tracing       # Run the executable; generates .ppm output to output/ directory
```

Once code exists, verify output with: `file output/*.ppm` and inspect the image header.

## High-Level Architecture

### Modular Design

The project follows clean separation of concerns across modules:

- **vec3** (`src/vec3.h/c`): 3D math primitives (add, sub, dot, cross, length, normalize). Includes random generation utilities (`random_double()`, `random_unit_vector()`).
- **ray** (`src/ray.h`): Simple ray struct (origin point3, direction vec3).
- **hittable** (`src/hittable.h`): Abstract interface using function pointers for object collision. All geometry implements this.
- **sphere** (`src/sphere.h/c`): Concrete hittable implementing ray-sphere intersection.
- **material** (`src/material.h/c`): Scatter logic with three implementations:
  - *Lambertian*: Diffuse scattering
  - *Metal*: Reflective with fuzziness parameter
  - *Dielectric*: Glass using Snell's law + Schlick's fresnel approximation
- **camera** (`src/camera.h/c`): Viewport and FOV logic; handles depth-of-field (defocus blur).
- **main.c**: Entry point; recursive ray color calculation; PPM output; scene generation and render loop.

### Data Flow

1. Camera generates rays through viewport
2. Main loop casts rays into the scene
3. Ray intersects hittable objects (spheres)
4. Hit records material; material scatters ray or absorbs it
5. Recursive bounces (depth limit ~50) accumulate color
6. Final pixel color is written to PPM file

### Memory Management

- Central `hittable_list` manages all scene objects
- Cleanup function called at end of `main()` to free dynamic allocations
- Prefer stack-allocated small structs (vec3, ray) over heap allocations where possible

## Code Conventions

### Style

- **Naming**: Functions & variables use `snake_case`; types use `snake_case_t` suffix; constants use `UPPER_SNAKE_CASE`
- **Formatting**: K&R style braces, 4-space indentation, max 100 character lines
- **Comments**: Concise; use `//` for lines, `/* */` for blocks
- **Documentation**: Function documentation in `.h` files describing parameters and return values
- **Memory**: Use `const` for immutable parameters; `static` for internal functions; `inline` for small math utilities

### Development Workflow

1. Implement feature (see implementation roadmap in `Specification.md`)
2. Compile with warnings enabled (`make`)
3. Create/update unit tests in `tests/`
4. Generate visual validation PPM and verify output
5. Commit with Conventional Commits format (see below)
6. Wait for human review before next step

### Commits

Follow Conventional Commits. Format:
```
<type>(<scope>): <subject>

<body>

Co-authored-by: Copilot <223556219+Copilot@users.noreply.github.com>
```

Types: `feat`, `fix`, `docs`, `refactor`, `style`, `test`, `chore`. One logical change per commit.

**Example:**
```
feat(materials): implement glass material with Schlick's approximation

Add Dielectric material type with proper refraction using Snell's law
and Schlick's fresnel approximation for reflectance.

Co-authored-by: Copilot <223556219+Copilot@users.noreply.github.com>
```

## Implementation Roadmap

The project follows a 6-step progression:

1. **Foundation & Math**: vec3 module with vector operations and random utilities
2. **Image Output & Ray**: Basic PPM generation; gradient background from ray direction
3. **Geometry & Intersections**: Hittable interface; sphere primitive; render single sphere
4. **Antialiasing & Camera**: Multi-sampling (MSAA); camera module; defocus blur
5. **Materials (Scatter System)**: Lambertian, Metal, Dielectric implementations
6. **Final Scene**: Recursive bouncing; complex scene generation; 1200x800 render

See `Specification.md` section 5 for detailed prompts for each step.

## Important Files

| File | Purpose |
|------|---------|
| `COPILOT.md` | Agent interaction protocol and detailed coding standards |
| `Specification.md` | Complete technical requirements and implementation prompts |
| `SESSION.md` | Official session log; record all prompt interactions here |
| `README.md` | Project overview and context |
| `src/` | Source code (created incrementally) |
| `tests/` | Unit tests (created as needed) |
| `output/` | Rendered PPM images for visual validation |

## Session Logging

All interactions must be logged in `SESSION.md` in this format:

```
[timestamp] - [feature/task description]

**Prompt:**
[The exact prompt sent to the model]

**Model:** [Model name, e.g., Claude 3.5 Haiku]
```

## Ray Tracing References

The algorithms and concepts are based on "Ray Tracing in One Weekend" series (v4.0.2) by Peter Shirley, Trevor David Black, and Steve Hollasch. See https://raytracing.github.io/
