// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#define SYSTEM_PRIVATE_CORELIB

using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Resources;
using System.Runtime.CompilerServices;
using System.Threading;

#nullable enable

namespace System
{
    internal static partial class SR
    {
        private static readonly object _lock = new object();
        private static List<string>? _currentlyLoading;
        private static int _infinitelyRecursingCount;
#if SYSTEM_PRIVATE_CORELIB
        private static bool _resourceManagerInited;
#endif

        private static string InternalGetResourceString(string key)
        {
            if (key.Length == 0)
            {
                Debug.Fail("SR::GetResourceString with empty resourceKey.  Bug in caller, or weird recursive loading problem?");
                return key;
            }

            // We have a somewhat common potential for infinite
            // loops with mscorlib's ResourceManager.  If "potentially dangerous"
            // code throws an exception, we will get into an infinite loop
            // inside the ResourceManager and this "potentially dangerous" code.
            // Potentially dangerous code includes the IO package, CultureInfo,
            // parts of the loader, some parts of Reflection, Security (including
            // custom user-written permissions that may parse an XML file at
            // class load time), assembly load event handlers, etc.  Essentially,
            // this is not a bounded set of code, and we need to fix the problem.
            // Fortunately, this is limited to mscorlib's error lookups and is NOT
            // a general problem for all user code using the ResourceManager.

            // The solution is to make sure only one thread at a time can call
            // GetResourceString.  Also, since resource lookups can be
            // reentrant, if the same thread comes into GetResourceString
            // twice looking for the exact same resource name before
            // returning, we're going into an infinite loop and we should
            // return a bogus string.

            bool lockTaken = false;
            try
            {
                Monitor.Enter(_lock, ref lockTaken);

                // Are we recursively looking up the same resource?  Note - our backout code will set
                // the ResourceHelper's currentlyLoading stack to null if an exception occurs.
                if (_currentlyLoading != null && _currentlyLoading.Count > 0 && _currentlyLoading.LastIndexOf(key) >= 0)
                {
                    // We can start infinitely recursing for one resource lookup,
                    // then during our failure reporting, start infinitely recursing again.
                    // avoid that.
                    if (_infinitelyRecursingCount > 0)
                    {
                        return key;
                    }
                    _infinitelyRecursingCount++;

#if SYSTEM_PRIVATE_CORELIB
                    // Note: our infrastructure for reporting this exception will again cause resource lookup.
                    // This is the most direct way of dealing with that problem.
                    string message = $@"Encountered infinite recursion while looking up resource '{key}' in XXXX. Verify the installation of .NET is complete and does not need repairing, and that the state of the process has not become corrupted.";
                    Environment.FailFast(message);
#endif
                }

                _currentlyLoading ??= new List<string>();

#if SYSTEM_PRIVATE_CORELIB
                // Call class constructors preemptively, so that we cannot get into an infinite
                // loop constructing a TypeInitializationException.  If this were omitted,
                // we could get the Infinite recursion assert above by failing type initialization
                // between the Push and Pop calls below.
                if (!_resourceManagerInited)
                {
                    RuntimeHelpers.RunClassConstructor(typeof(ResourceManager).TypeHandle);
                    RuntimeHelpers.RunClassConstructor(typeof(ResourceReader).TypeHandle);
                    RuntimeHelpers.RunClassConstructor(Type.GetType("System.Resources.RuntimeResourceSet")!.TypeHandle);
                    RuntimeHelpers.RunClassConstructor(typeof(BinaryReader).TypeHandle);
                    _resourceManagerInited = true;
                }
#endif

                _currentlyLoading.Add(key); // Push

                string? s = ResourceManager.GetString(key, null);
                _currentlyLoading.RemoveAt(_currentlyLoading.Count - 1); // Pop

                Debug.Assert(s != null, $"Looking up resource '{key}' failed. Was your resource name misspelled? Did you rebuild after adding a resource?");
                return s ?? key;
            }
            catch
            {
                if (lockTaken)
                {
                    // Backout code - throw away potentially corrupt state
                    s_resourceManager = null;
                    _currentlyLoading = null;
                }
                throw;
            }
            finally
            {
                if (lockTaken)
                {
                    Monitor.Exit(_lock);
                }
            }
        }

        private static readonly bool s_usingResourceKeys = AppContext.TryGetSwitch("System.Resources.UseSystemResourceKeys", out bool usingResourceKeys) ? usingResourceKeys : false;

        // This method is used to decide if we need to append the exception message parameters to the message when calling SR.Format.
        // by default it returns the value of System.Resources.UseSystemResourceKeys AppContext switch or false if not specified.
        // Native code generators can replace the value this returns based on user input at the time of native code generation.
        // The trimming tools are also capable of replacing the value of this method when the application is being trimmed.
        internal static bool UsingResourceKeys() => s_usingResourceKeys;

        // We can optimize out the resource string blob if we can see all accesses to it happening
        // through the generated SR.XXX properties.
        // If a call to GetResourceString is left, the optimization gets defeated and we need to keep
        // the whole resource blob. It's important to keep this private. CoreCLR's CoreLib gets a free
        // pass because the VM needs to be able to call into this, but that's a known set of constants.
#if CORECLR || LEGACY_GETRESOURCESTRING_USER
        internal
#else
        private
#endif
        static string GetResourceString(string resourceKey)
        {
            if (UsingResourceKeys())
            {
                return resourceKey;
            }

            string? resourceString = null;
            try
            {
                resourceString =
#if SYSTEM_PRIVATE_CORELIB || NATIVEAOT
                    InternalGetResourceString(resourceKey);
#else
                    ResourceManager.GetString(resourceKey);
#endif
            }
            catch (MissingManifestResourceException) { }

            return resourceString!; // only null if missing resources
        }

#if LEGACY_GETRESOURCESTRING_USER
        internal
#else
        private
#endif
        static string GetResourceString(string resourceKey, string defaultString)
        {
            string resourceString = GetResourceString(resourceKey);

            return resourceKey == resourceString || resourceString == null ? defaultString : resourceString;
        }

        internal static string Format(string resourceFormat, object? p1)
        {
            if (UsingResourceKeys())
            {
                return string.Join(", ", resourceFormat, p1);
            }

            return string.Format(resourceFormat, p1);
        }

        internal static string Format(string resourceFormat, object? p1, object? p2)
        {
            if (UsingResourceKeys())
            {
                return string.Join(", ", resourceFormat, p1, p2);
            }

            return string.Format(resourceFormat, p1, p2);
        }

        internal static string Format(string resourceFormat, object? p1, object? p2, object? p3)
        {
            if (UsingResourceKeys())
            {
                return string.Join(", ", resourceFormat, p1, p2, p3);
            }

            return string.Format(resourceFormat, p1, p2, p3);
        }

        internal static string Format(string resourceFormat, params object?[]? args)
        {
            if (args != null)
            {
                if (UsingResourceKeys())
                {
                    return resourceFormat + ", " + string.Join(", ", args);
                }

                return string.Format(resourceFormat, args);
            }

            return resourceFormat;
        }

        internal static string Format(IFormatProvider? provider, string resourceFormat, object? p1)
        {
            if (UsingResourceKeys())
            {
                return string.Join(", ", resourceFormat, p1);
            }

            return string.Format(provider, resourceFormat, p1);
        }

        internal static string Format(IFormatProvider? provider, string resourceFormat, object? p1, object? p2)
        {
            if (UsingResourceKeys())
            {
                return string.Join(", ", resourceFormat, p1, p2);
            }

            return string.Format(provider, resourceFormat, p1, p2);
        }

        internal static string Format(IFormatProvider? provider, string resourceFormat, object? p1, object? p2, object? p3)
        {
            if (UsingResourceKeys())
            {
                return string.Join(", ", resourceFormat, p1, p2, p3);
            }

            return string.Format(provider, resourceFormat, p1, p2, p3);
        }

        internal static string Format(IFormatProvider? provider, string resourceFormat, params object?[]? args)
        {
            if (args != null)
            {
                if (UsingResourceKeys())
                {
                    return resourceFormat + ", " + string.Join(", ", args);
                }

                return string.Format(provider, resourceFormat, args);
            }

            return resourceFormat;
        }
        
    }
}
