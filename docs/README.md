<h1>Deribit API Client</h1>
<hr><h2>Step-by-Step Build and Run Instructions</h2>
<h3>1. <strong>Clone the Repository (if not done already)</strong></h3>
<p>If you haven't cloned the repository yet, run the following command:</p>
<pre><code class="language-bash">git clone &lt;repository-url&gt;
cd &lt;repository-directory&gt;
</code></pre>
<h3>2. <strong>Clean Up Previous Builds (Optional)</strong></h3>
<p>To ensure a clean build, remove any previous build artifacts:</p>
<pre><code class="language-bash">rm -rf build
</code></pre>
<h3>3. <strong>Create the Build Directory</strong></h3>
<p>Create a new <code>build</code> directory where the compiled files will be placed:</p>
<pre><code class="language-bash">mkdir build
</code></pre>
<h3>4. <strong>Navigate to the Build Directory</strong></h3>
<p>Move into the <code>build</code> directory:</p>
<pre><code class="language-bash">cd build
</code></pre>
<h3>5. <strong>Run CMake to Configure the Project</strong></h3>
<p>Use <strong>CMake</strong> to configure the project and generate the necessary build files:</p>
<pre><code class="language-bash">cmake ..
</code></pre>
<p>This command will check for required dependencies and create the Makefile.</p>
<h3>6. <strong>Build the Project</strong></h3>
<p>After configuration, run <strong>make</strong> to compile the project:</p>
<pre><code class="language-bash">make
</code></pre>
<h3>7. <strong>Run the Project</strong></h3>
<p>Once the build process is complete, you can run the compiled executable:</p>
<pre><code class="language-bash">./DeribitTradingSystem
</code></pre>
<hr>
<h3>Additional Notes:</h3>
<ul>
<li><strong>Dependencies:</strong> Ensure that all dependencies (like <code>libcurl</code>, <code>nlohmann/json</code>, <code>boost</code>, etc.) are installed on your system before starting the build process.</li>
<li><strong>Error Handling:</strong> If you encounter errors during the build process, check the output for missing dependencies or configuration issues. You may need to install missing packages.</li>
</ul><h2>Contact</h2>
<hr><p><span style="margin-right: 30px;"></span><a href="linkedin.com/in/swarajchouriwar"><img target="_blank" src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/linkedin/linkedin-original.svg" style="width: 10%;"></a><span style="margin-right: 30px;"></span><a href="github.com/swaraj2442"><img target="_blank" src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/github/github-original.svg" style="width: 10%;"></a></p>