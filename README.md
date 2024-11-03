# rocketleague-ai-coach

Here’s an outline of the testing stage and the planned stages, formatted for your GitHub README. This will provide a structured overview of the development process and link relevant resources.

---

## Rocket League AI Coach

An AI-powered plugin for Rocket League, using local large language models (LLMs) to offer real-time, strategic coaching feedback based on in-game data.

### Overview

This project leverages the BakkesMod SDK to collect gameplay metrics, with a local LLaMA-7B model to provide actionable coaching tips. The project uses a Flask server to interact with the model and provides feedback to players using Windows TTS.

### Current Stage: Testing Basic Plugin Setup

#### Testing Stage

1. **Basic Plugin Verification**:
   - Confirm the plugin loads in BakkesMod and logs events every second to the console and log file.
   - Check for recurring log entries in both the BakkesMod console (press `F6` to open) and the BakkesMod log file (in `BakkesMod\logs`).

2. **Testing Interval**:
   - Verify the `SetTimeout` function in BakkesMod is working as intended and logs messages every second without memory leaks or performance issues.

3. **Expected Output**:
   - A new log message every second, formatted as `"Event logged at <timestamp>"`.

4. **Next Steps**:
   - Once basic logging is confirmed, proceed to the following stages to add game data polling, model integration, and feedback generation.

---

### Planned Stages

#### Stage 1: Setting Up and Polling Game Data

1. **Objective**: Capture essential gameplay metrics, such as player position and boost level, using the BakkesMod SDK.
2. **Resources**:
   - [BakkesMod SDK Documentation](https://bakkesplugins.com/docs/getting-started/).
3. **Implementation**:
   - Update the plugin to capture game data every 5 seconds.
   - Log captured data to the console to verify correctness before sending it to the model.

#### Stage 2: Installing and Configuring Local LLaMA-7B Model

1. **Objective**: Set up the LLaMA-7B model locally to serve as the AI coach.
2. **Resources**:
   - [Hugging Face Transformers](https://huggingface.co/docs/transformers/installation).
   - [Meta’s LLaMA Model on Hugging Face](https://huggingface.co/meta-llama).
3. **Implementation**:
   - Install Python dependencies (Transformers, Flask, PyTorch) and configure the LLaMA-7B model.
   - Write a Flask server (`server.py`) to handle requests from the plugin and generate feedback based on gameplay data.

#### Stage 3: Integrating the Plugin with Local Model API

1. **Objective**: Send gameplay data from the BakkesMod plugin to the Flask server and receive coaching feedback.
2. **Resources**:
   - [Flask Documentation](https://flask.palletsprojects.com/en/latest/).
   - [cURL Documentation](https://curl.se/docs/).
3. **Implementation**:
   - Use cURL in the plugin to send a JSON payload with game data to the local API.
   - Parse the model’s response and log it to verify communication.

#### Stage 4: Adding Real-Time Audio Feedback with Windows TTS

1. **Objective**: Use Windows TTS to play the model’s feedback aloud, providing real-time coaching.
2. **Resources**:
   - [Microsoft Speech API (SAPI) Documentation](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee125663(v=vs.85)).
3. **Implementation**:
   - Use Windows SAPI to convert the text response into spoken audio.
   - Test different voices and timing adjustments to match real-time gameplay needs.

#### Stage 5: Fine-Tuning LLaMA for Specific Rocket League Coaching Tips (Optional)

1. **Objective**: Improve the relevance and clarity of model responses by fine-tuning LLaMA on Rocket League-specific data.
2. **Resources**:
   - [Hugging Face Fine-Tuning Guide](https://huggingface.co/docs/transformers/training).
3. **Implementation**:
   - Prepare a dataset of Rocket League scenarios and coaching responses.
   - Fine-tune LLaMA and integrate the updated model for more tailored advice.

---

### Future Plans

- **Optimization**: Explore quantization techniques for faster inference.
- **Additional Metrics**: Integrate more gameplay metrics (e.g., speed, ball position) for richer feedback.
- **Advanced Audio**: Experiment with different TTS engines or pre-recorded coaching audio for variety.

For the full project source code, development updates, and additional details, please visit the [GitHub repository](https://github.com/scottleedavis/rocketleague-ai-coach/).

--- 

This README section will provide a clear roadmap for the plugin’s development and ensure each stage is properly linked to the relevant resources. Let me know if you need any additional specifics!
