using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Text.Json.Serialization;
using System.Text.RegularExpressions;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class Policy
    {
        private bool regexApproveCompiledDone = false;

        private List<Regex> regexApprovedCompiled;

        private bool regexRejectCompiledDone = false;

        private List<Regex> regexRejectCompiled;

        public bool AppliesToSetOperations { get; set; }

        public bool AppliesToChangeOperations { get; set; }

        public int MinimumLength { get; set; }

        public List<string> RegexApprove { get; set; } = new List<string>();

        public List<string> RegexReject { get; set; } = new List<string>();

        public bool PreventUseOfUsername { get; set; }

        public bool PreventUseOfFullName { get; set; }

        public bool NormalizedBannedWordChecking { get; set; }

        public bool NormalizedCompromisedPasswordChecking { get; set; }

        public bool CompromisedPasswordChecking { get; set; }

        public int MaximumRepeatedCharacters { get; set; }

        public List<string> BannedSequences { get; set; } = new List<string>();

        public ComplexityPointsPolicy PointsBasedComplexity { get; set; }

        public List<ComplexityThresholdPolicy> LengthBasedComplexity { get; set; } = new List<ComplexityThresholdPolicy>();

        [JsonIgnore]
        public List<Regex> RegexApprovedCompiled
        {
            get
            {
                if (!regexApproveCompiledDone)
                {
                    regexApprovedCompiled = new List<Regex>();

                    if (this.RegexApprove != null && this.RegexApprove.Count > 0)
                    {
                        foreach (string r in this.RegexApprove)
                        {
                            regexApprovedCompiled.Add(new Regex(r, RegexOptions.IgnoreCase));
                        }
                    }

                    regexApproveCompiledDone = true;
                }

                return regexApprovedCompiled;
            }
        }

        [JsonIgnore]
        public List<Regex> RegexRejectCompiled
        {
            get
            {
                if (!regexRejectCompiledDone)
                {
                    regexRejectCompiled = new List<Regex>();

                    if (this.RegexReject != null && this.RegexReject.Count > 0)
                    {
                        foreach (string r in this.RegexReject)
                        {
                            regexRejectCompiled.Add(new Regex(r, RegexOptions.IgnoreCase));
                        }
                    }

                    regexRejectCompiledDone = true;
                }

                return regexRejectCompiled;
            }
        }
    }
}