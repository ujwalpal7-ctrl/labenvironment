# LabEnvironment

Laboratorio virtual para que estudiantes escolares midan y comparen **temperatura (°C)** y **humedad relativa (%)** en tres ambientes: sala, exterior y cerca de una ventana.

- **Web App (demo):** https://ujwalpal7-ctrl.github.io/labenvironment/
- **Asignatura:** Proyecto de Innovación Tecnológica · **Grupo:** Paw Patrol 2 · **Temática:** Ambiente

## Estado del proyecto (Gate 4)

| Componente | Estado |
|---|---|
| Web App (interfaz, flujo de 4 pasos, gráficos, estadística) | ✅ Implementada |
| Sensor DHT22 | ⚠️ **Simulado** por software (ruido gaussiano) |
| Firmware ESP32 + Wi-Fi | 🕓 Escrito, **no probado en hardware** |

> Los datos de la demo son simulados. El error porcentual se calcula contra un valor de referencia nominal editable, por lo que refleja el ruido del modelo y no el error de un sensor real.

## Cómo usar la Web App

1. Abre el enlace de la demo en un smartphone o computador (no requiere instalación).
2. **Paso 1:** presiona *Conectar al sensor*.
3. **Paso 2:** elige un lugar y presiona *Iniciar medición (3 s)*. Repite hasta tener **5 mediciones por lugar**.
4. **Paso 3:** revisa la tabla (promedio, desviación estándar, error %) y los gráficos comparativos.
5. **Paso 4:** lee las conclusiones y compáralas con tu hipótesis.

Ejecución local: descarga `index.html` y ábrelo en el navegador (requiere internet para cargar Chart.js).

## Modelo de datos

- Cada lectura: `x = x0 + ε`, con ε ~ N(0, σ²). σ = 0,3 °C (temperatura) y 1,5 % (humedad).
- Una medición = promedio de 3 lecturas (1 por segundo).
- Promedio `x̄`, desviación estándar muestral `s` y error `|x̄ − x_ref| / x_ref × 100`.

## Estructura del repositorio

```
index.html                         Web App (HTML + CSS + JS + Chart.js)
README.md                          Este archivo
firmware/labenvironment_esp32/     Código Arduino para ESP32 + DHT22 (planificado)
docs/Informe_Tecnico_LabEnvironment.pdf
```

## Hardware planificado

Arquitectura: `DHT22 → ESP32 → Wi-Fi → Smartphone → Web App → Gráficos`

- DHT22: VCC → 3V3, DATA → GPIO 4 (pull-up 10 kΩ), GND → GND.
- Arduino IDE, placa *ESP32 Dev Module*, librerías *DHT sensor library* y *Adafruit Unified Sensor*.
- La ESP32 crea la red `LabEnvironment-ESP32` y entrega `{"t":22.1,"h":45.3}` en `http://192.168.4.1/data`.
- El DHT22 mide como máximo cada 2 s (0,5 Hz).
- Limitación: una página HTTPS (GitHub Pages) no puede leer un servidor HTTP local; la lectura real requerirá abrir la app localmente o servirla desde la propia ESP32.

## Equipo y roles

| Integrante | Rol |
|---|---|
| Dimitry Quezada | Product Owner (Líder), Software/Datos y Pedagógico |
| Vicente Vergara | Responsable Científico y Hardware/Instrumentación |
| Ujwal Pal | UX/UI |
